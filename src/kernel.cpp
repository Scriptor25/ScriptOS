#include "scriptos/format.h"

#include <limine.h>
#include <scriptos/bitmap.h>
#include <scriptos/fpu.h>
#include <scriptos/gdt.h>
#include <scriptos/idt.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

#define LIMINE_REQUEST       __attribute__((used, section(".limine_requests"))) static volatile
#define LIMINE_REQUEST_START __attribute__((used, section(".limine_requests_start"))) static volatile
#define LIMINE_REQUEST_END   __attribute__((used, section(".limine_requests_end"))) static volatile

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

NORETURN static void freeze(void)
{
    for (;;)
        asm volatile("hlt");
}

extern "C" NORETURN void kmain(void)
{
    serial::Initialize();
    fpu::Initialize();
    gdt::Initialize();
    idt::Initialize();

    if (!LIMINE_BASE_REVISION_SUPPORTED)
    {
        serial::Write("limine base revision not supported\r\n");
        asm volatile("int $0x69");
    }

    if (!bootloader_info_request.response)
    {
        serial::Write("no bootloader info response\r\n");
        asm volatile("int $0x69");
    }

    if (!firmware_type_request.response)
    {
        serial::Write("no firmware type response\r\n");
        asm volatile("int $0x69");
    }

    if (!hhdm_request.response)
    {
        serial::Write("no hhdm response\r\n");
        asm volatile("int $0x69");
    }

    if (!framebuffer_request.response)
    {
        serial::Write("no framebuffer response\r\n");
        asm volatile("int $0x69");
    }

    if (!memmap_request.response)
    {
        serial::Write("no memmap response\r\n");
        asm volatile("int $0x69");
    }

    if (!mp_request.response)
    {
        serial::Write("no mp response\r\n");
        asm volatile("int $0x69");
    }

    print(serial::Write,
          "bootloader: %s, %s\r\n",
          bootloader_info_request.response->name,
          bootloader_info_request.response->version);

    serial::Write("firmware type: ");
    switch (firmware_type_request.response->firmware_type)
    {
    case LIMINE_FIRMWARE_TYPE_X86BIOS:
        serial::Write("X86BIOS");
        break;
    case LIMINE_FIRMWARE_TYPE_UEFI32:
        serial::Write("UEFI32");
        break;
    case LIMINE_FIRMWARE_TYPE_UEFI64:
        serial::Write("UEFI64");
        break;
    case LIMINE_FIRMWARE_TYPE_SBI:
        serial::Write("SBI");
        break;
    default:
        serial::Write("?");
        break;
    }
    serial::Write("\r\n");

    auto framebuffer = framebuffer_request.response->framebuffers[0];
    auto framebuffer_address = static_cast<u32*>(framebuffer->address);
    auto framebuffer_width = framebuffer->width;
    auto framebuffer_height = framebuffer->height;

    for (u64 j = 0; j < framebuffer_height; ++j)
        for (u64 i = 0; i < framebuffer_width; ++i)
        {
            auto r = static_cast<float>(i) / static_cast<float>(framebuffer_width - 1);
            auto g = static_cast<float>(j) / static_cast<float>(framebuffer_height - 1);
            auto b = 0.2f;

            auto ir = static_cast<u32>(r * 255.999);
            auto ig = static_cast<u32>(g * 255.999);
            auto ib = static_cast<u32>(b * 255.999);

            auto color = (ir & 0xff) << 16 | (ig & 0xff) << 8 | (ib & 0xff);

            framebuffer_address[j * framebuffer_width + i] = color;
        }

    usize end_address = 0;
    for (usize i = 0; i < memmap_request.response->entry_count; ++i)
    {
        auto [base, length, type] = *memmap_request.response->entries[i];
        if ((type == LIMINE_MEMMAP_USABLE || type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) && (end_address < (base + length)))
            end_address = base + length;

        print(serial::Write, "%016X, %016X, ", base, length);
        switch (type)
        {
        case LIMINE_MEMMAP_USABLE:
            serial::Write("USABLE");
            break;
        case LIMINE_MEMMAP_RESERVED:
            serial::Write("RESERVED");
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            serial::Write("ACPI RECLAIMABLE");
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            serial::Write("ACPI NVS");
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            serial::Write("BAD MEMORY");
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            serial::Write("BOOTLOADER RECLAIMABLE");
            break;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            serial::Write("EXECUTABLE AND MODULES");
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            serial::Write("FRAMEBUFFER");
            break;
        }
        serial::Write("\r\n");
    }

    print(serial::Write, "memory size: %016X (%u KiB)\r\n", end_address, end_address / 1024);

    u8* bitmap_buffer = nullptr;
    usize largest_region_length = 0;

    for (usize i = 0; i < memmap_request.response->entry_count; ++i)
    {
        auto [base, length, type] = *memmap_request.response->entries[i];

        if (type != LIMINE_MEMMAP_USABLE)
            continue;

        if (length < largest_region_length)
            continue;

        bitmap_buffer = reinterpret_cast<u8*>(base);
        largest_region_length = length;
    }

    auto page_count = end_address / 0x1000;

    Bitmap bitmap(bitmap_buffer + hhdm_request.response->offset, page_count);
    bitmap.Clear();

    for (usize i = 0; i < memmap_request.response->entry_count; ++i)
    {
        auto [base, length, type] = *memmap_request.response->entries[i];

        bitmap.Fill(base, length / 0x1000, type == LIMINE_MEMMAP_USABLE);
    }

    freeze();
}
