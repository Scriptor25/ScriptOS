#include <limine.h>
#include <scriptos/bitmap.h>
#include <scriptos/format.h>
#include <scriptos/fpu.h>
#include <scriptos/gdt.h>
#include <scriptos/idt.h>
#include <scriptos/memory.h>
#include <scriptos/renderer.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

#define LIMINE_REQUEST                                                 \
    __attribute__((used, section(".limine_requests"))) static volatile
#define LIMINE_REQUEST_START                                                 \
    __attribute__((used, section(".limine_requests_start"))) static volatile
#define LIMINE_REQUEST_END                                                 \
    __attribute__((used, section(".limine_requests_end"))) static volatile

#define NORETURN __attribute__((noreturn))

LIMINE_REQUEST_START LIMINE_REQUESTS_START_MARKER;
LIMINE_REQUEST LIMINE_BASE_REVISION(3);
LIMINE_REQUEST limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST limine_firmware_type_request firmware_type_request = {
    .id = LIMINE_FIRMWARE_TYPE_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST limine_mp_request mp_request = {
    .id = LIMINE_MP_REQUEST,
    .revision = 0,
    .response = nullptr,
    .flags = LIMINE_MP_X2APIC,
};
LIMINE_REQUEST_END LIMINE_REQUESTS_END_MARKER;

NORETURN static void halt()
{
    for (;;)
        asm volatile("cli; hlt");
}

NORETURN static void error(cstr message)
{
    Print(serial::Write, "%s\r\n", message);
    asm volatile("int $0x69");
    halt();
}

static Renderer renderer;

static void write_char(int c)
{
    serial::Write(c);
    renderer.NextChar(c);
}

static void print_system_information()
{
    Print(write_char,
          "bootloader: %s, %s\r\n",
          bootloader_info_request.response->name,
          bootloader_info_request.response->version);

    cstr firmware_type_string;
    switch (firmware_type_request.response->firmware_type)
    {
    case LIMINE_FIRMWARE_TYPE_X86BIOS:
        firmware_type_string = "X86BIOS";
        break;
    case LIMINE_FIRMWARE_TYPE_UEFI32:
        firmware_type_string = "UEFI32";
        break;
    case LIMINE_FIRMWARE_TYPE_UEFI64:
        firmware_type_string = "UEFI64";
        break;
    case LIMINE_FIRMWARE_TYPE_SBI:
        firmware_type_string = "SBI";
        break;
    default:
        firmware_type_string = "?";
        break;
    }

    Print(write_char, "firmware type: %s\r\n", firmware_type_string);

    Print(write_char, "\r\n");

    Print(write_char, "framebuffers:\r\n");

    for (usize i = 0; i < framebuffer_request.response->framebuffer_count; ++i)
    {
        auto framebuffer = framebuffer_request.response->framebuffers[i];

        Print(write_char,
              " %4d: %016X, %dx%d\r\n",
              i,
              framebuffer->address,
              framebuffer->width,
              framebuffer->height);
    }

    Print(write_char, "\r\n");

    Print(write_char, "memory:\r\n");

    usize end_address = 0;

    for (usize i = 0; i < memmap_request.response->entry_count; ++i)
    {
        auto entry = memmap_request.response->entries[i];

        if ((entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
            && (end_address < (entry->base + entry->length)))
            end_address = entry->base + entry->length;

        cstr type_string;
        switch (entry->type)
        {
        case LIMINE_MEMMAP_USABLE:
            type_string = "USABLE";
            break;
        case LIMINE_MEMMAP_RESERVED:
            type_string = "RESERVED";
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            type_string = "ACPI RECLAIMABLE";
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            type_string = "ACPI NVS";
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            type_string = "BAD MEMORY";
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            type_string = "BOOTLOADER RECLAIMABLE";
            break;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            type_string = "EXECUTABLE AND MODULES";
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            type_string = "FRAMEBUFFER";
            break;
        }

        Print(write_char, " %016X, %016X, %s\r\n", entry->base, entry->length, type_string);
    }

    Print(write_char, "total size: %016X (%u KiB)\r\n", end_address, end_address / 1024);

    Print(write_char, "\r\n");

    Print(write_char, " index | processor_id | lapic_id | goto_address     \r\n");
    Print(write_char, "-------+--------------+----------+------------------\r\n");

    for (usize i = 0; i < mp_request.response->cpu_count; ++i)
    {
        auto cpu = mp_request.response->cpus[i];

        Print(write_char, " %-4d  | %-4d         | %-4d     | %016X \r\n", i, cpu->processor_id, cpu->lapic_id, cpu->goto_address);
    }
}

struct control_block
{
    void (*target)(void*);
    void* data;
};

extern "C" NORETURN void kmain()
{
    serial::Initialize();
    fpu::Initialize();
    gdt::Initialize();
    idt::Initialize();

    if (!LIMINE_BASE_REVISION_SUPPORTED)
        error("limine base revision not supported");

    if (!bootloader_info_request.response)
        error("no bootloader info response");

    if (!firmware_type_request.response)
        error("no firmware type response");

    if (!hhdm_request.response)
        error("no hhdm response");

    if (!framebuffer_request.response)
        error("no framebuffer response");

    if (!memmap_request.response)
        error("no memmap response");

    if (!mp_request.response)
        error("no mp response");

    {
        auto framebuffer = framebuffer_request.response->framebuffers[0];
        renderer.Initialize({
            framebuffer->address,
            framebuffer->width,
            framebuffer->height,
        });
        renderer.SetForeground(0xffffffff);
        renderer.SetBackground(0xff121212);
        renderer.Reset();
        renderer.Clear();
    }

    print_system_information();

    u8* bitmap_buffer = nullptr;
    usize max_length = 0;

    usize end_address = 0;

    for (usize i = 0; i < memmap_request.response->entry_count; ++i)
    {
        auto entry = memmap_request.response->entries[i];

        if (entry->type != LIMINE_MEMMAP_USABLE && entry->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
            continue;

        if (end_address < (entry->base + entry->length))
            end_address = entry->base + entry->length;

        if (entry->length < max_length)
            continue;

        bitmap_buffer = reinterpret_cast<u8*>(entry->base);
        max_length = entry->length;
    }

    auto page_count = end_address / 0x1000;

    Bitmap bitmap(bitmap_buffer + hhdm_request.response->offset, page_count);
    bitmap.Clear();

    for (usize i = 0; i < memmap_request.response->entry_count; ++i)
    {
        auto entry = memmap_request.response->entries[i];

        bitmap.Fill(entry->base, entry->length / 0x1000, entry->type == LIMINE_MEMMAP_USABLE);
    }

    Print(write_char, "generated memory bitmap\r\n");

    halt();
}
