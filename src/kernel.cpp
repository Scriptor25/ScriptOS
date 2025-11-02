#include <efi.h>
#include <limine.h>
#include <scriptos/acpi.h>
#include <scriptos/ahci.h>
#include <scriptos/asm.h>
#include <scriptos/bitmap.h>
#include <scriptos/boot/limine.h>
#include <scriptos/fpu.h>
#include <scriptos/gdt.h>
#include <scriptos/graphics.h>
#include <scriptos/idt.h>
#include <scriptos/io.h>
#include <scriptos/kernel.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/pci.h>
#include <scriptos/pic.h>
#include <scriptos/pit.h>
#include <scriptos/print.h>
#include <scriptos/processor.h>
#include <scriptos/range.h>
#include <scriptos/serial.h>
#include <scriptos/task/schedule.h>
#include <scriptos/task/task.h>
#include <scriptos/tss.h>
#include <scriptos/types.h>

kernel::InstanceT kernel::Instance = {
    .Allocator = nullptr,
    .Renderer = nullptr,
};

__attribute__((noreturn)) static void error(
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
    {
        hlt();
    }
}

static void initialize_allocator()
{
    u8* physical_buffer = nullptr;
    usize max_length = 0;
    usize memory_end = 0;

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
        max_length = entry->length;
    }

    auto virtual_buffer = paging::PhysicalToVirtual<u8*>(physical_buffer);

    auto page_count = memory_end / PAGE_SIZE;
    Bitmap bitmap(virtual_buffer, page_count);

    bitmap.Clear();
    for (auto entry : memmap)
    {
        bitmap.Fill(entry->base / PAGE_SIZE, entry->length / PAGE_SIZE, entry->type != LIMINE_MEMMAP_USABLE);
    }

    bitmap.Fill(0, 0x100, true);
    bitmap.Fill(reinterpret_cast<uptr>(physical_buffer) / PAGE_SIZE, page_count / 8 + 1, true);

    kernel::Instance.Allocator = reinterpret_cast<paging::PageFrameAllocator*>(virtual_buffer + memory_end / 8);
    *kernel::Instance.Allocator = { bitmap };
}

static void initialize_renderer()
{
    auto framebuffer = framebuffer_request.response->framebuffers[0];
    auto back_buffer = memory::Allocate(framebuffer->pitch * framebuffer->height);

    kernel::Instance.Renderer = memory::Allocate<graphics::Renderer>(
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
            kprintf("              [%c] | %-3u | %-5u | %-6u | %-5u | %02X    \r\n", active ? '*' : ' ', mode->bpp, mode->width, mode->height, mode->pitch, mode->memory_model);
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

        kprintf(" %016X | %016X | %-22s \r\n", entry->base, entry->length, type_string);
    }

    kputs("\r\n");

    kprintf("total size: %016X (%u KiB)\r\n", end_address, end_address / 1024);

    kputs("\r\n");

    kputs(" processor id | lapic id | goto address     \r\n");
    kputs("--------------+----------+------------------\r\n");

    Range cpus(mp_request.response->cpus, mp_request.response->cpu_count);
    for (auto cpu : cpus)
    {
        kprintf(" %-4u         | %-4u     | %016X \r\n", cpu->processor_id, cpu->lapic_id, cpu->goto_address);
    }

    kputs("\r\n");

    if (efi_system_table_request.response)
    {
        auto system_table = reinterpret_cast<EFI_SYSTEM_TABLE*>(
            efi_system_table_request.response->address);
        paging::MapPage(system_table, system_table);

        kprintf("efi system table: %016X\r\n", system_table);

        auto firmware_vendor = system_table->FirmwareVendor;
        paging::MapPage(firmware_vendor, firmware_vendor);

        kprintf("firmware vendor: %h\r\n", firmware_vendor);
    }
}

static void print_mcfg(const acpi::MCFG* mcfg)
{
    for (auto& entry : *mcfg)
    {
        auto base_address = reinterpret_cast<const u8*>(entry.BaseAddress);
        const pci::RootIterable root(base_address, entry.StartBus, entry.EndBus);

        for (const auto [bus_index, bus] : root)
        {
            for (const auto [device_index, device] : bus)
            {
                for (const auto [function_index, function] : device)
                {
                    if (function->DeviceID == 0xFFFF || function->VendorID == 0xFFFF)
                    {
                        continue;
                    }

                    auto device_descriptor = pci::GetDeviceDescriptor(
                        function->BaseClass,
                        function->SubClass,
                        function->ProgIF);
                    auto vendor_name = pci::GetVendorName(function->VendorID);
                    auto device_name = pci::GetDeviceName(
                        function->VendorID,
                        function->DeviceID);

                    kprintf("[ %02X:%02X:%02X ] ", bus_index, device_index, function_index);
                    if (device_descriptor)
                    {
                        kprintf("%s, ", device_descriptor);
                    }
                    else
                    {
                        kprintf(
                            "%02X-%02X-%02X, ",
                            function->BaseClass,
                            function->SubClass,
                            function->ProgIF);
                    }
                    if (vendor_name)
                    {
                        kprintf("%s, ", vendor_name);
                    }
                    else
                    {
                        kprintf("%04X, ", function->VendorID);
                    }
                    if (device_name)
                    {
                        kprintf("%s", device_name);
                    }
                    else
                    {
                        kprintf("%04X", function->DeviceID);
                    }
                    kputs("\r\n");
                }
            }
        }
    }
}

static void find_ahci(const acpi::MCFG* mcfg)
{
    for (auto& entry : *mcfg)
    {
        auto base_address = reinterpret_cast<const u8*>(entry.BaseAddress);
        const pci::RootIterable root(base_address, entry.StartBus, entry.EndBus);

        for (const auto [bus_index, bus] : root)
        {
            for (const auto [device_index, device] : bus)
            {
                for (const auto [function_index, function] : device)
                {
                    if (function->BaseClass != 0x01 || function->SubClass != 0x06 || function->ProgIF != 0x01)
                    {
                        continue;
                    }

                    auto ahci = reinterpret_cast<const pci::PCIDevice*>(function);
                    auto abar = reinterpret_cast<ahci::hba::MEM_T*>(ahci->BAR5 & 0xFFFFF000);
                    paging::MapPage(abar, abar, true, true, false, false, true, false);

                    abar->GHC.AE = true;

                    for (unsigned i = 0; i < abar->CAP.NP; ++i)
                    {
                        if (!(abar->PI & (1 << i)))
                        {
                            continue;
                        }

                        auto port = abar->PCR + i;

                        if (port->SATAStatus.DeviceDetection != ahci::hba::HBA_PORT_DET_PRESENT)
                        {
                            continue;
                        }
                        if (port->SATAStatus.InterfacePowerManagement != ahci::hba::HBA_PORT_IPM_ACTIVE)
                        {
                            continue;
                        }

                        auto base_address = kernel::Instance.Allocator->AllocatePhysicalPages(0x10);
                        paging::MapPages(base_address, base_address, 0x10, true, true);

                        if (!ahci::Initialize(abar, port, reinterpret_cast<uptr>(base_address)))
                        {
                            kprintf("failed to rebase port %u\r\n", i);
                        }

                        auto buffer = kernel::Instance.Allocator->AllocatePhysicalPage();
                        paging::MapPage(buffer, buffer, true, true);

                        memory::Fill(buffer, 0, PAGE_SIZE);

                        switch (*reinterpret_cast<const u32*>(&port->Signature))
                        {
                        case ahci::hba::HBA_PORT_SIG_ATA:
                            kprintf("located ATA drive at port %u\r\n", i);
                            if (!ahci::ReadATA(abar, port, 0, 1, buffer))
                            {
                                kprintf("failed to read from port %u\r\n", i);
                            }
                            break;

                        case ahci::hba::HBA_PORT_SIG_ATAPI:
                            kprintf("located ATAPI drive at port %u\r\n", i);
                            if (!ahci::ReadATAPI(abar, port, 0, 1, buffer))
                            {
                                kprintf("failed to read from port %u\r\n", i);
                            }
                            break;

                        case ahci::hba::HBA_PORT_SIG_SEMB:
                            kprintf("located SEMB drive at port %u\r\n", i);
                            break;

                        case ahci::hba::HBA_PORT_SIG_PM:
                            kprintf("located PM drive at port %u\r\n", i);
                            break;

                        default:
                            continue;
                        }

                        kprintmem(buffer, 0x80);

                        kernel::Instance.Allocator->FreePage(buffer);
                    }
                }
            }
        }
    }
}

struct print_task_t
{
    cstr message;
};

// static void print_task(void* arg)
// {
//     auto t = reinterpret_cast<const print_task_t*>(arg);
//
//     for (;;)
//     {
//         cli();
//         kputs(t->message);
//         sti();
//     }
// }

static void ping_pong_task(void* arg)
{
    task::Task* task;

    if (arg)
    {
        cli();
        kputs("pong\r\n");
        sti();

        task = task::CreateTask("ping", 0, ping_pong_task, nullptr);
    }
    else
    {
        cli();
        kputs("ping\r\n");
        sti();

        task = task::CreateTask("pong", 0, ping_pong_task, reinterpret_cast<void*>(0xDEADBEEF));
    }

    task::EnqueueTask(task);
}

__attribute__((noreturn)) static void kernel_task(void* arg)
{
    (void) arg;

    // {
    //     auto arg = memory::Allocate<print_task_t>();
    //     arg->message = "A";
    //     auto task = task::CreateTask("print a", 1, print_task, arg);
    //     task::EnqueueTask(task);
    // }
    // {
    //     auto arg = memory::Allocate<print_task_t>();
    //     arg->message = "B";
    //     auto task = task::CreateTask("print b", 2, print_task, arg);
    //     task::EnqueueTask(task);
    // }
    // {
    //     auto arg = memory::Allocate<print_task_t>();
    //     arg->message = "C";
    //     auto task = task::CreateTask("print c", 3, print_task, arg);
    //     task::EnqueueTask(task);
    // }
    // {
    //     auto arg = memory::Allocate<print_task_t>();
    //     arg->message = "D";
    //     auto task = task::CreateTask("print d", 4, print_task, arg);
    //     task::EnqueueTask(task);
    // }

    {
        auto task = task::CreateTask("ping", 0, ping_pong_task, nullptr);
        task::EnqueueTask(task);
    }

    for (;;)
    {
        cli();
        kflush();
        sti();
        hlt();
    }
}

extern "C" __attribute__((noreturn)) void kmain()
{
    cli();

    serial::InitializeAll();

    fpu::Initialize();
    gdt::Initialize();
    idt::Initialize();

    pic::Disable();
    pic::Remap(0x20, 0x28);

    pit::Initialize(500);
    pic::ClearMask(0);

    sti();

    if (!LIMINE_BASE_REVISION_SUPPORTED)
    {
        error("limine base revision not supported");
    }

    if (!bootloader_info_request.response)
    {
        error("no bootloader info response");
    }

    if (!firmware_type_request.response)
    {
        error("no firmware type response");
    }

    if (!hhdm_request.response)
    {
        error("no hhdm response");
    }

    if (!framebuffer_request.response)
    {
        error("no framebuffer response");
    }

    if (!memmap_request.response)
    {
        error("no memmap response");
    }

    if (!mp_request.response)
    {
        error("no mp response");
    }

    if (!rsdp_request.response)
    {
        error("no rsdp response");
    }

    paging::Initialize(hhdm_request.response->offset);

    initialize_allocator();

    auto kernel_stack = kernel::Instance.Allocator->AllocatePhysicalPage();
    paging::MapPage(kernel_stack, kernel_stack, true, true);
    tss::Initialize(kernel_stack, nullptr, nullptr);

    memory::InitializeHeap(0x400000);

    processor::Initialize(0);

    initialize_renderer();

    print_system_information();

    auto xsdp = reinterpret_cast<acpi::XSDP*>(rsdp_request.response->address);
    paging::MapPage(xsdp, xsdp);

    const acpi::MCFG* mcfg;
    switch (xsdp->Revision)
    {
    case 0:
    case 1:
    {
        auto rsdt = reinterpret_cast<const acpi::RSDT*>(xsdp->RSDT_Address);
        paging::MapPage(rsdt, rsdt);

        mcfg = rsdt->Find<acpi::MCFG>("MCFG");
        if (!mcfg)
        {
            error("no mcfg table");
        }

        break;
    }

    case 2:
    {
        auto xsdt = reinterpret_cast<const acpi::XSDT*>(xsdp->XSDT_Address);
        paging::MapPage(xsdt, xsdt);

        mcfg = xsdt->Find<acpi::MCFG>("MCFG");
        if (!mcfg)
        {
            error("no mcfg table");
        }

        break;
    }

    default:
        error("unsupported rsdp revision %d", xsdp->Revision);
    }

    print_mcfg(mcfg);

    // TODO: read drivers from disk
    // find_ahci(mcfg);
    (void) find_ahci;

    kflush();

    auto task = task::CreateTask("kernel", 0, kernel_task, nullptr);
    task::EnqueueTask(task);

    for (;;)
    {
        hlt();
    }
}
