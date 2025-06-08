#include <efi.h>
#include <limine.h>
#include <scriptos/bitmap.h>
#include <scriptos/format.h>
#include <scriptos/fpu.h>
#include <scriptos/gdt.h>
#include <scriptos/idt.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/range.h>
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
LIMINE_REQUEST limine_efi_system_table_request efi_system_table_request = {
    .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST limine_paging_mode_request paging_mode_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .response = nullptr,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL,
    .max_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
    .min_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
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
static paging::PageFrameAllocator allocator;

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

    Print(write_char, " address          | bpp | width | height | pitch | model \r\n");
    Print(write_char, "------------------+-----+-------+--------+-------+-------\r\n");

    Range framebuffers(framebuffer_request.response->framebuffers,
                       framebuffer_request.response->framebuffer_count);
    for (auto framebuffer : framebuffers)
    {
        Print(write_char,
              " %016X |     |       |        |       |       \r\n",
              framebuffer->address);

        Range modes(framebuffer->modes, framebuffer->mode_count);
        for (auto mode : modes)
        {
            auto active = mode->bpp == framebuffer->bpp
                       && mode->width == framebuffer->width
                       && mode->height == framebuffer->height
                       && mode->pitch == framebuffer->pitch
                       && mode->memory_model == framebuffer->memory_model;
            Print(write_char, "              [%c] | %-3u | %-5u | %-6u | %-5u | %02X    \r\n", active ? '*' : ' ', mode->bpp, mode->width, mode->height, mode->pitch, mode->memory_model);
        }
    }

    Print(write_char, "\r\n");

    Print(write_char, " base             | length           | type                   \r\n");
    Print(write_char, "------------------+------------------+------------------------\r\n");

    usize end_address = 0;

    Range memmap(memmap_request.response->entries, memmap_request.response->entry_count);
    for (auto entry : memmap)
    {
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

        Print(write_char, " %016X | %016X | %-22s \r\n", entry->base, entry->length, type_string);
    }

    Print(write_char, "\r\n");

    Print(write_char, "total size: %016X (%u KiB)\r\n", end_address, end_address / 1024);

    Print(write_char, "\r\n");

    Print(write_char, " processor id | lapic id | goto address     \r\n");
    Print(write_char, "--------------+----------+------------------\r\n");

    Range cpus(mp_request.response->cpus, mp_request.response->cpu_count);
    for (auto cpu : cpus)
        Print(write_char, " %-4u         | %-4u     | %016X \r\n", cpu->processor_id, cpu->lapic_id, cpu->goto_address);

    Print(write_char, "\r\n");
}

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

    paging::Initialize(hhdm_request.response->offset);

    u8* bitmap_buffer = nullptr;
    usize max_length = 0;
    usize end_address = 0;

    Range memmap(memmap_request.response->entries, memmap_request.response->entry_count);
    for (auto entry : memmap)
    {
        if (entry->type != LIMINE_MEMMAP_USABLE && entry->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
            continue;

        if (end_address < (entry->base + entry->length))
            end_address = entry->base + entry->length;

        if (entry->length < max_length)
            continue;

        bitmap_buffer = reinterpret_cast<u8*>(entry->base);
        max_length = entry->length;
    }

    {
        auto page_count = end_address / PAGE_SIZE;

        Bitmap bitmap(reinterpret_cast<u8*>(paging::PhysicalToVirtual(bitmap_buffer)), page_count);
        bitmap.Clear();
        for (auto entry : memmap)
            bitmap.Fill(entry->base / PAGE_SIZE, entry->length / PAGE_SIZE, entry->type != LIMINE_MEMMAP_USABLE);

        bitmap.Fill(reinterpret_cast<uptr>(bitmap_buffer) / PAGE_SIZE, page_count / 8 + 1, true);

        allocator = { bitmap };
    }

    {
        auto framebuffer = framebuffer_request.response->framebuffers[0];

        auto buffer_size = framebuffer->pitch * framebuffer->height;
        auto page_count = (buffer_size / PAGE_SIZE) + 1;

        auto buffer_physical = allocator.AllocatePhysicalPages(page_count);
        auto buffer_virtual = paging::PhysicalToVirtual(buffer_physical);

        paging::MapPages(allocator, buffer_virtual, buffer_physical, page_count, true, true);

        renderer.Initialize(framebuffer->address,
                            buffer_virtual,
                            framebuffer->width,
                            framebuffer->height,
                            framebuffer->pitch,
                            framebuffer->bpp);
        renderer.SetForeground(0xffffffff);
        renderer.SetBackground(0xff121212);
        renderer.Reset();
        renderer.Clear();
    }

    print_system_information();

    if (efi_system_table_request.response)
    {
        auto physical_system_table = reinterpret_cast<void*>(efi_system_table_request
                                                                 .response->address);
        auto virtual_system_table = reinterpret_cast<EFI_SYSTEM_TABLE*>(paging::PhysicalToVirtual(physical_system_table));

        paging::MapPage(allocator, virtual_system_table, physical_system_table);

        Print(write_char, "efi system table: %016X\r\n", virtual_system_table);

        auto physical_firmware_vendor = virtual_system_table->FirmwareVendor;
        auto virtual_firmware_vendor = reinterpret_cast<CHAR16*>(paging::PhysicalToVirtual(physical_firmware_vendor));

        paging::MapPage(allocator, virtual_firmware_vendor, physical_firmware_vendor);

        Print(write_char, "firmware vendor: %h\r\n", virtual_firmware_vendor);
    }

    renderer.SwapBuffers();

    halt();
}
