#include <efi.h>
#include <limine.h>
#include <scriptos/acpi.h>
#include <scriptos/ahci.h>
#include <scriptos/bitmap.h>
#include <scriptos/fpu.h>
#include <scriptos/gdt.h>
#include <scriptos/idt.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/pci.h>
#include <scriptos/print.h>
#include <scriptos/range.h>
#include <scriptos/renderer.h>
#include <scriptos/serial.h>
#include <scriptos/tss.h>
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
LIMINE_REQUEST limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
    .response = nullptr,
};
LIMINE_REQUEST_END LIMINE_REQUESTS_END_MARKER;

NORETURN static void halt()
{
    for (;;)
        asm volatile("cli ; hlt");
}

NORETURN static void error(
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    PrintV(format, ap);
    va_end(ap);
    Print("\r\n");
    asm volatile("int $0x69");
    halt();
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
            continue;

        if (memory_end < (entry->base + entry->length))
            memory_end = entry->base + entry->length;

        if (entry->length < max_length)
            continue;

        physical_buffer = reinterpret_cast<u8*>(entry->base);
        max_length = entry->length;
    }

    auto virtual_buffer = paging::PhysicalToVirtual<u8*>(physical_buffer);

    auto page_count = memory_end / PAGE_SIZE;
    Bitmap bitmap(virtual_buffer, page_count);

    bitmap.Clear();
    for (auto entry : memmap)
        bitmap.Fill(entry->base / PAGE_SIZE, entry->length / PAGE_SIZE, entry->type != LIMINE_MEMMAP_USABLE);

    bitmap.Fill(0, 0x100, true);
    bitmap.Fill(reinterpret_cast<uptr>(physical_buffer) / PAGE_SIZE, page_count / 8 + 1, true);

    paging::KernelAllocator = reinterpret_cast<paging::PageFrameAllocator*>(virtual_buffer + memory_end / 8);
    *paging::KernelAllocator = { bitmap };
}

static void initialize_renderer()
{
    auto framebuffer = framebuffer_request.response->framebuffers[0];
    auto back_buffer = memory::Allocate(framebuffer->pitch * framebuffer->height);

    KernelRenderer = memory::MakeUnique<Renderer>(
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
    KernelRenderer->SetForeground(0xffffff);
    KernelRenderer->SetBackground(0x121212);
    KernelRenderer->Reset();
    KernelRenderer->Clear();
}

static void print_system_information()
{
    Print(
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

    Print("firmware type: %s\r\n", firmware_type_string);

    Print("\r\n");

    Print(" address          | bpp | width | height | pitch | model \r\n");
    Print("------------------+-----+-------+--------+-------+-------\r\n");

    Range framebuffers(
        framebuffer_request.response->framebuffers,
        framebuffer_request.response->framebuffer_count);
    for (auto framebuffer : framebuffers)
    {
        Print(" %016X |     |       |        |       |       \r\n", framebuffer->address);

        Range modes(framebuffer->modes, framebuffer->mode_count);
        for (auto mode : modes)
        {
            auto active = mode->bpp == framebuffer->bpp
                       && mode->width == framebuffer->width
                       && mode->height == framebuffer->height
                       && mode->pitch == framebuffer->pitch
                       && mode->memory_model == framebuffer->memory_model;
            Print("              [%c] | %-3u | %-5u | %-6u | %-5u | %02X    \r\n", active ? '*' : ' ', mode->bpp, mode->width, mode->height, mode->pitch, mode->memory_model);
        }
    }

    Print("\r\n");

    Print(" base             | length           | type                   \r\n");
    Print("------------------+------------------+------------------------\r\n");

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

        Print(" %016X | %016X | %-22s \r\n", entry->base, entry->length, type_string);
    }

    Print("\r\n");

    Print("total size: %016X (%u KiB)\r\n", end_address, end_address / 1024);

    Print("\r\n");

    Print(" processor id | lapic id | goto address     \r\n");
    Print("--------------+----------+------------------\r\n");

    Range cpus(mp_request.response->cpus, mp_request.response->cpu_count);
    for (auto cpu : cpus)
        Print(" %-4u         | %-4u     | %016X \r\n", cpu->processor_id, cpu->lapic_id, cpu->goto_address);

    Print("\r\n");

    if (efi_system_table_request.response)
    {
        auto system_table = reinterpret_cast<EFI_SYSTEM_TABLE*>(
            efi_system_table_request.response->address);
        paging::MapPage(system_table, system_table);

        Print("efi system table: %016X\r\n", system_table);

        auto firmware_vendor = system_table->FirmwareVendor;
        paging::MapPage(firmware_vendor, firmware_vendor);

        Print("firmware vendor: %h\r\n", firmware_vendor);
    }
}

static void print_mcfg(const acpi::MCFG* mcfg)
{
    for (auto& entry : *mcfg)
    {
        auto base_address = reinterpret_cast<const u8*>(entry.BaseAddress);
        const pci::RootIterable root(base_address, entry.StartBus, entry.EndBus);

        for (const auto [bus_index, bus] : root)
            for (const auto [device_index, device] : bus)
                for (const auto [function_index, function] : device)
                {
                    if (function->DeviceID == 0xFFFF || function->VendorID == 0xFFFF)
                        continue;

                    auto device_descriptor = pci::GetDeviceDescriptor(
                        function->BaseClass,
                        function->SubClass,
                        function->ProgIF);
                    auto vendor_name = pci::GetVendorName(function->VendorID);
                    auto device_name = pci::GetDeviceName(
                        function->VendorID,
                        function->DeviceID);

                    Print("[ %02X:%02X:%02X ] ", bus_index, device_index, function_index);
                    if (device_descriptor)
                        Print("%s, ", device_descriptor);
                    else
                        Print(
                            "%02X-%02X-%02X, ",
                            function->BaseClass,
                            function->SubClass,
                            function->ProgIF);
                    if (vendor_name)
                        Print("%s, ", vendor_name);
                    else
                        Print("%04X, ", function->VendorID);
                    if (device_name)
                        Print("%s", device_name);
                    else
                        Print("%04X", function->DeviceID);
                    Print("\r\n");
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
            for (const auto [device_index, device] : bus)
                for (const auto [function_index, function] : device)
                {
                    if (function->BaseClass != 0x01 || function->SubClass != 0x06 || function->ProgIF != 0x01)
                        continue;

                    auto ahci = reinterpret_cast<const pci::PCIDevice*>(function);
                    auto abar = reinterpret_cast<ahci::hba::MEM_T*>(ahci->BAR5 & 0xFFFFF000);
                    paging::MapPage(abar, abar, true, true, false, false, true, false);

                    abar->GHC.AE = true;

                    for (unsigned i = 0; i < abar->CAP.NP; ++i)
                    {
                        if (!(abar->PI & (1 << i)))
                            continue;

                        auto port = abar->PCR + i;

                        if (port->SATAStatus.DeviceDetection != ahci::hba::HBA_PORT_DET_PRESENT)
                            continue;
                        if (port->SATAStatus.InterfacePowerManagement != ahci::hba::HBA_PORT_IPM_ACTIVE)
                            continue;

                        auto base_address = paging::KernelAllocator->AllocatePhysicalPages(0x10);
                        paging::MapPages(base_address, base_address, 0x10, true, true);

                        if (!ahci::Initialize(abar, port, reinterpret_cast<uptr>(base_address)))
                            Print("failed to rebase port %u\r\n", i);

                        auto buffer = paging::KernelAllocator->AllocatePhysicalPage();
                        paging::MapPage(buffer, buffer, true, true);

                        memory::Fill(buffer, 0, PAGE_SIZE);

                        switch (*reinterpret_cast<const u32*>(&port->Signature))
                        {
                        case ahci::hba::HBA_PORT_SIG_ATA:
                            Print("located ATA drive at port %u\r\n", i);
                            if (!ahci::ReadATA(abar, port, 0, 1, buffer))
                                Print("failed to read from port %u\r\n", i);
                            break;

                        case ahci::hba::HBA_PORT_SIG_ATAPI:
                            Print("located ATAPI drive at port %u\r\n", i);
                            if (!ahci::ReadATAPI(abar, port, 0, 1, buffer))
                                Print("failed to read from port %u\r\n", i);
                            break;

                        case ahci::hba::HBA_PORT_SIG_SEMB:
                            Print("located SEMB drive at port %u\r\n", i);
                            break;

                        case ahci::hba::HBA_PORT_SIG_PM:
                            Print("located PM drive at port %u\r\n", i);
                            break;

                        default:
                            continue;
                        }

                        paging::KernelAllocator->FreePage(buffer);
                    }
                }
    }
}

extern "C" NORETURN void kmain()
{
    serial::InitializeAll();

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

    if (!rsdp_request.response)
        error("no rsdp response");

    paging::Initialize(hhdm_request.response->offset);

    initialize_allocator();

    auto kernel_stack = paging::KernelAllocator->AllocatePhysicalPage();
    paging::MapPage(kernel_stack, kernel_stack, true, true);
    tss::Initialize(kernel_stack, nullptr, nullptr);

    memory::InitializeHeap(0x400000);

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
            error("no mcfg table");

        break;
    }

    case 2:
    {
        auto xsdt = reinterpret_cast<const acpi::XSDT*>(xsdp->XSDT_Address);
        paging::MapPage(xsdt, xsdt);

        mcfg = xsdt->Find<acpi::MCFG>("MCFG");
        if (!mcfg)
            error("no mcfg table");

        break;
    }

    default:
        error("unsupported rsdp revision %d", xsdp->Revision);
    }

    print_mcfg(mcfg);
    find_ahci(mcfg);

    // TODO: read drivers from disk

    Flush();

    halt();
}
