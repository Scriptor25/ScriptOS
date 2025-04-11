#include <scriptos/kernel/ahci.hpp>
#include <scriptos/kernel/hba.hpp>
#include <scriptos/kernel/pci.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/debug.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/util.hpp>

void PCI::EnumeratePCI(ACPI::MCFG_Header *mcfg)
{
    beg_tbl(5, 80, "vendor", "device", "class", "subclass", "prog if");

    mcfg->ForEach(
        [](const ACPI::MCFG_Entry &entry)
        {
            for (auto bus = entry.StartPCIBus; bus < entry.EndPCIBus; ++bus)
                EnumerateBus(entry.BaseAddressLo, bus);
        });

    end_tbl();
}

void PCI::EnumerateBus(uptr base_address, uptr bus)
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto offset = bus << 20;
    auto bus_address = base_address + offset;

    if (!ptm.MapPage(reinterpret_cast<void *>(bus_address), reinterpret_cast<void *>(bus_address)))
        return;

    auto device_header = reinterpret_cast<Device_Header *>(bus_address);

    if (device_header->DeviceID == 0x0000 || device_header->DeviceID == 0xffff || device_header->VendorID == 0x0000 || device_header->VendorID == 0xffff)
        return;

    for (usize device = 0; device < 32; ++device)
        EnumerateDevice(bus_address, device);
}

void PCI::EnumerateDevice(uptr bus_address, uptr device)
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto offset = device << 15;
    auto device_address = bus_address + offset;

    if (!ptm.MapPage(reinterpret_cast<void *>(device_address), reinterpret_cast<void *>(device_address)))
        return;

    auto device_header = reinterpret_cast<Device_Header *>(device_address);

    if (device_header->DeviceID == 0x0000 || device_header->DeviceID == 0xffff || device_header->VendorID == 0x0000 || device_header->VendorID == 0xffff)
        return;

    EnumerateFunction(device_address, 0);

    if (!device_header->HeaderType.MF)
        return;

    for (usize function = 1; function < 8; ++function)
        EnumerateFunction(device_address, function);
}

static void enumerate_ahci(PCI::Device_Type_0 *device)
{
    auto &ptm = PageTableManager::GetKernelInstance();
    auto &pfa = PageFrameAllocator::GetKernelInstance();

    auto ahci_bar = reinterpret_cast<PCI::Memory_BaseAddress_Register *>(&device->BaseAddress5);
    auto ahci_base_phys = ahci_bar->BaseAddress << 4;
    auto ahci_base = reinterpret_cast<void *>(ahci_base_phys);

    if (!ptm.MapPage(ahci_base, ahci_base))
        return;

    debug("mapped AHCI MMIO at %p", ahci_base);

    auto hba = reinterpret_cast<HBA::Memory *>(ahci_base);
    hba->GHC |= (1 << 31);

    debug("AHCI version: %08X", hba->VS);
    debug("AHCI ports implemented: %08X", hba->PI);

    for (usize i = 0; i < 32; ++i)
    {
        if (!(hba->PI & (1 << i)))
            continue;

        auto &port = hba->Ports[i];

        auto ipm = (port.SSTS >> 8) & 0x0F;
        auto det = port.SSTS & 0x0F;

        if (det != 3 || ipm != 1)
        {
            debug("port %d: no active device (det=%02X, ipm=%02X)", i, det, ipm);
            continue;
        }

        switch (port.SIG)
        {
        case SATA_SIG_ATA:
        {
            debug("port %d: ATA drive", i);

            auto clb = pfa.RequestPage();
            auto fb = pfa.RequestPage();

            port.CLB = reinterpret_cast<uptr>(clb);
            port.CLBU = 0;

            port.FB = reinterpret_cast<uptr>(fb);
            port.FBU = 0;

            break;
        }

        case SATA_SIG_ATAPI:
        {
            debug("port %d: ATAPI drive", i);
            break;
        }

        case SATA_SIG_SEMB:
            debug("port %d: SEMB drive", i);
            break;

        case SATA_SIG_PM:
            debug("port %d: PM drive", i);
            break;

        default:
            debug("port %d: signature %08X", i, port.SIG);
            break;
        }
    }
}

void PCI::EnumerateFunction(uptr device_address, uptr function)
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto offset = function << 12;
    auto function_address = device_address + offset;

    if (!ptm.MapPage(reinterpret_cast<void *>(function_address), reinterpret_cast<void *>(function_address)))
        return;

    auto device_header = reinterpret_cast<Device_Header *>(function_address);

    auto vendor_id = device_header->VendorID;
    auto device_id = device_header->DeviceID;

    if (device_id == 0x0000 || device_id == 0xffff || vendor_id == 0x0000 || vendor_id == 0xffff)
        return;

    auto class_code = device_header->ClassCode;
    auto subclass = device_header->Subclass;
    auto prog_if = device_header->ProgIF;

    auto vendor_name = GetVendorName(vendor_id);
    auto device_name = GetDeviceName(vendor_id, device_id);

    auto class_code_desc = GetDeviceDescriptor(class_code);
    auto subclass_desc = GetDeviceDescriptor(class_code, subclass);
    auto prog_if_desc = GetDeviceDescriptor(class_code, subclass, prog_if);

    if (class_code == 0x01 && subclass == 0x06 && prog_if == 0x01)
    {
        auto device = reinterpret_cast<Device_Type_0 *>(function_address);
        enumerate_ahci(device);
    }

    char buf[8];
    int len;

    len = uitoa(buf, vendor_id, 16, 4, true);
    buf[len] = 0;
    next_col(vendor_name ? vendor_name : buf);

    len = uitoa(buf, device_id, 16, 4, true);
    buf[len] = 0;
    next_col(device_name ? device_name : buf);

    next_col(class_code_desc ? class_code_desc : nullptr);

    next_col(subclass_desc ? subclass_desc : nullptr);

    next_col(prog_if_desc ? prog_if_desc : nullptr);
}
