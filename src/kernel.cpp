#include <efi.h>
#include <limine.h>
#include <scriptos/acpi.h>
#include <scriptos/asm.h>
#include <scriptos/bitmap.h>
#include <scriptos/common.h>
#include <scriptos/fpu.h>
#include <scriptos/gdt.h>
#include <scriptos/graphics.h>
#include <scriptos/idt.h>
#include <scriptos/io.h>
#include <scriptos/kernel.h>
#include <scriptos/limine.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/pci.h>
#include <scriptos/pic.h>
#include <scriptos/pit.h>
#include <scriptos/print.h>
#include <scriptos/processor.h>
#include <scriptos/range.h>
#include <scriptos/serial.h>
#include <scriptos/task.h>
#include <scriptos/tss.h>
#include <scriptos/types.h>

kernel::KernelInstance kernel::Instance = {
    .Allocator = nullptr,
    .Renderer  = nullptr,
};

NORETURN static void error(
    cstr format,
    ...)
{
    cli();

    va_list ap;
    va_start(ap, format);
    vkprintf(format, ap);
    va_end(ap);

    kputs("\r\n");

    INT(0x69);

    for (;;)
        hlt();
}

static void initialize_allocator()
{
    u8* physical_buffer = nullptr;
    usize max_length    = 0;
    usize memory_end    = 0;

    Range memmap(memmap_request.response->entries, memmap_request.response->entry_count);
    for (auto entry : memmap)
    {
        if (entry->type != LIMINE_MEMMAP_USABLE && entry->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
        {
            continue;
        }

        if (memory_end < (entry->base + entry->length))
        {
            memory_end = entry->base + entry->length;
        }

        if (entry->length < max_length)
        {
            continue;
        }

        physical_buffer = reinterpret_cast<u8*>(entry->base);
        max_length      = entry->length;
    }

    auto virtual_buffer = kernel::PhysicalToVirtual<u8*>(physical_buffer);

    auto page_count = memory_end / PAGE_SIZE;
    kernel::Bitmap bitmap(virtual_buffer, page_count);

    bitmap.Clear();
    for (auto entry : memmap)
    {
        bitmap.Fill(entry->base / PAGE_SIZE, entry->length / PAGE_SIZE, entry->type != LIMINE_MEMMAP_USABLE);
    }

    bitmap.Fill(0, 0x100, true);
    bitmap.Fill(reinterpret_cast<uptr>(physical_buffer) / PAGE_SIZE, page_count / 8 + 1, true);

    kernel::Instance.Allocator = reinterpret_cast<kernel::PageFrameAllocator*>(virtual_buffer + memory_end / 8);
    *kernel::Instance.Allocator = { bitmap };
}

static void initialize_renderer()
{
    auto framebuffer = framebuffer_request.response->framebuffers[0];
    auto back_buffer = kernel::Allocate(framebuffer->pitch * framebuffer->height);

    kernel::Instance.Renderer = kernel::Allocate<kernel::BasicRenderer>(
        framebuffer->address,
        back_buffer,
        framebuffer->width,
        framebuffer->height,
        framebuffer->pitch,
        framebuffer->bpp,
        framebuffer->red_mask_shift,
        framebuffer->red_mask_size,
        framebuffer->green_mask_shift,
        framebuffer->green_mask_size,
        framebuffer->blue_mask_shift,
        framebuffer->blue_mask_size);

    kernel::Instance.Renderer->SetForeground(0xffffff);
    kernel::Instance.Renderer->SetBackground(0x121212);
    kernel::Instance.Renderer->Reset();
    kernel::Instance.Renderer->Clear();
}

static void print_system_information()
{
    kprintf(
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

    kprintf("firmware type: %s\r\n", firmware_type_string);

    kputs("\r\n");

    kputs(" address          | bpp | width | height | pitch | model \r\n");
    kputs("------------------+-----+-------+--------+-------+-------\r\n");

    Range framebuffers(
        framebuffer_request.response->framebuffers,
        framebuffer_request.response->framebuffer_count);
    for (auto framebuffer : framebuffers)
    {
        kprintf(
            " %016llx |     |       |        |       |       \r\n",
            framebuffer->address);

        Range modes(framebuffer->modes, framebuffer->mode_count);
        for (auto mode : modes)
        {
            auto active = mode->bpp == framebuffer->bpp
                       && mode->width == framebuffer->width
                       && mode->height == framebuffer->height
                       && mode->pitch == framebuffer->pitch
                       && mode->memory_model == framebuffer->memory_model;
            kprintf("              [%c] | %-3u | %-5u | %-6u | %-5u | %02x    \r\n", active ? '*' : ' ', mode->bpp, mode->width, mode->height, mode->pitch, mode->memory_model);
        }
    }

    kputs("\r\n");

    kputs(" base             | length           | type                   \r\n");
    kputs("------------------+------------------+------------------------\r\n");

    usize end_address = 0;

    Range memmap(memmap_request.response->entries, memmap_request.response->entry_count);
    for (auto entry : memmap)
    {
        if ((entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
            && (end_address < (entry->base + entry->length)))
        {
            end_address = entry->base + entry->length;
        }

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

        kprintf(" %016llx | %016llx | %-22s \r\n", entry->base, entry->length, type_string);
    }

    kputs("\r\n");

    kprintf("total size: %016llx (%u KiB)\r\n", end_address, end_address / 0x400);

    kputs("\r\n");

    kputs(" processor id | lapic id | goto address     \r\n");
    kputs("--------------+----------+------------------\r\n");

    Range cpus(mp_request.response->cpus, mp_request.response->cpu_count);
    for (auto cpu : cpus)
    {
        kprintf(" %-4u         | %-4u     | %016llx \r\n", cpu->processor_id, cpu->lapic_id, cpu->goto_address);
    }

    kputs("\r\n");

    if (efi_system_table_request.response)
    {
        auto system_table = reinterpret_cast<EFI_SYSTEM_TABLE*>(
            efi_system_table_request.response->address);
        kernel::MapPage(system_table, system_table);

        kprintf("efi system table: %016llx\r\n", system_table);

        auto firmware_vendor = system_table->FirmwareVendor;
        kernel::MapPage(firmware_vendor, firmware_vendor);

        kprintf("firmware vendor: %h\r\n", firmware_vendor);
    }
}

static void print_mcfg(const acpi::Mcfg* mcfg)
{
    for (auto& entry : *mcfg)
    {
        auto base_address = reinterpret_cast<const u8*>(entry.BaseAddress);
        const kernel::PciIterable root(base_address, entry.StartBus, entry.EndBus);

        for (const auto [bus_index, bus] : root)
        {
            for (const auto [device_index, device] : bus)
            {
                for (const auto [function_index, function] : device)
                {
                    if (function->DeviceID == 0xFFFF || function->VendorID == 0xFFFF)
                        continue;

                    auto device_descriptor = kernel::GetPciDeviceDescriptor(
                        function->BaseClass,
                        function->SubClass,
                        function->ProgIF);
                    auto vendor_name = kernel::GetPciVendorName(function->VendorID);
                    auto device_name = kernel::GetPciDeviceName(
                        function->VendorID,
                        function->DeviceID);

                    kprintf("[ %02x:%02x:%02x ] ", bus_index, device_index, function_index);
                    if (device_descriptor)
                        kprintf("%s, ", device_descriptor);
                    else
                        kprintf(
                            "%02x-%02x-%02x, ",
                            function->BaseClass,
                            function->SubClass,
                            function->ProgIF);
                    if (vendor_name)
                        kprintf("%s, ", vendor_name);
                    else
                        kprintf("%04x, ", function->VendorID);
                    if (device_name)
                        kprintf("%s", device_name);
                    else
                        kprintf("%04x", function->DeviceID);
                    kputs("\r\n");
                }
            }
        }
    }
}

static void ping_pong_task(void* arg)
{
    if (arg)
    {
        cli();
        kputs("pong\r\n");
        kflush();
        sti();
    }
    else
    {
        cli();
        kputs("ping\r\n");
        kflush();
        sti();

        auto task = kernel::CreateTask("pong", 0, ping_pong_task, reinterpret_cast<void*>(0xDEADBEEF));
        kernel::EnqueueTask(task);
    }
}

static void kernel_task(void* /* arg */)
{
    auto task = kernel::CreateTask("ping", 0, ping_pong_task, nullptr);
    kernel::EnqueueTask(task);
}

extern "C" NORETURN void kmain()
{
    cli();

    kernel::InitializeAllSerial();

    kernel::InitializeFPU();
    kernel::InitializeGDT();
    kernel::InitializeIDT();

    kernel::DisablePic();
    kernel::RemapPic(0x20, 0x28);

    kernel::InitializePit(500);
    kernel::ClearPicMask(0);

    sti();

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
    if (!rsdp_request.response)
        error("no rsdp response");

    kernel::InitializePaging(hhdm_request.response->offset);

    initialize_allocator();

    auto kernel_stack = kernel::Instance.Allocator->AllocatePhysicalPage();
    kernel::MapPage(kernel_stack, kernel_stack, true);
    kernel::InitializeTss(kernel_stack, nullptr, nullptr);

    kernel::InitializeHeap(0x400000);

    Range cpus(mp_request.response->cpus, mp_request.response->cpu_count);
    for (auto cpu : cpus)
        kernel::InitializeProcessorState(cpu->processor_id);

    initialize_renderer();

    print_system_information();

    auto xsdp = reinterpret_cast<acpi::ExtendedSystemDescriptorPointer*>(
        rsdp_request.response->address);
    kernel::MapPage(xsdp, xsdp);

    const acpi::Mcfg* mcfg;
    switch (xsdp->Revision)
    {
    case 0:
    case 1:
    {
        auto rsdt = reinterpret_cast<const acpi::SystemDescriptorTable*>(xsdp->RsdtAddress);
        kernel::MapPage(rsdt, rsdt);

        mcfg = rsdt->Find<acpi::Mcfg>("MCFG");
        if (!mcfg)
            error("no mcfg table");

        break;
    }

    case 2:
    {
        auto xsdt = reinterpret_cast<const acpi::ExtendedSystemDescriptorTable*>(xsdp->XsdtAddress);
        kernel::MapPage(xsdt, xsdt);

        mcfg = xsdt->Find<acpi::Mcfg>("MCFG");
        if (!mcfg)
            error("no mcfg table");

        break;
    }

    default:
        error("unsupported rsdp revision %d", xsdp->Revision);
    }

    print_mcfg(mcfg);

    kflush();

    auto task = kernel::CreateTask("kernel", 0, kernel_task, nullptr);
    kernel::EnqueueTask(task);

    for (;;)
        hlt();
}
