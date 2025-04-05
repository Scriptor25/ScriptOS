#include <scriptos/kernel/pci.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/print.hpp>

cstr PCI::GetDeviceDescriptor(u8 class_code, u8 subclass, u8 prog_if)
{
    switch (class_code)
    {
    case 0x00:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Non-VGA Compatible Unclassified Device" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "VGA Compatible Unclassified Device" : nullptr;
        case 0x05:
            return prog_if == 0xFF ? "Image Co-Processor" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Unclassified Device" : nullptr;
        default:
            return nullptr;
        }
    case 0x01:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "SCSI Storage Controller" : nullptr;
        case 0x01:
            switch (prog_if)
            {
            case 0x00:
                return "ISA Compatibility Mode-Only Controller";
            case 0x05:
                return "PCI Native Mode-Only Controller";
            case 0x0A:
                return "ISA Compatibility Mode Controller with Dual-Channel PCI Native Mode Support";
            case 0x0F:
                return "PCI Native Mode Controller with Dual-Channel ISA Compatibility Mode Support";
            case 0x80:
                return "ISA Compatibility Mode-Only Controller with Bus Mastering";
            case 0x85:
                return "PCI Native Mode-Only Controller with Bus Mastering";
            case 0x8A:
                return "ISA Compatibility Mode Controller with Dual-Channel PCI Native Mode and Bus Mastering Support";
            case 0x8F:
                return "PCI Native Mode Controller with Dual-Channel ISA Compatibility Mode and Bus Mastering Support";
            case 0xFF:
                return "IDE Interface";
            default:
                return nullptr;
            }
        case 0x02:
            return prog_if == 0xFF ? "Floppy Disk Controller" : nullptr;
        case 0x03:
            return prog_if == 0xFF ? "IPI Bus Controller" : nullptr;
        case 0x04:
            return prog_if == 0xFF ? "RAID Bus Controller" : nullptr;
        case 0x05:
            switch (prog_if)
            {
            case 0x20:
                return "ADMA Single Stepping";
            case 0x30:
                return "ADMA Continuous Operation";
            case 0xFF:
                return "ATA Controller";
            default:
                return nullptr;
            }
        case 0x06:
            switch (prog_if)
            {
            case 0x00:
                return "Vendor Specific";
            case 0x01:
                return "AHCI 1.0";
            case 0x02:
                return "Serial Storage Bus";
            case 0xFF:
                return "SATA Controller";
            default:
                return nullptr;
            }
        case 0x07:
            switch (prog_if)
            {
            case 0x01:
                return "Serial Storage Bus";
            case 0xFF:
                return "Serial Attached SCSI Controller";
            default:
                return nullptr;
            }
        case 0x08:
            switch (prog_if)
            {
            case 0x01:
                return "NVMHCI";
            case 0x02:
                return "NVM Express";
            case 0xFF:
                return "Non-Volatile Memory Controller";
            default:
                return nullptr;
            }
        case 0x09:
            switch (prog_if)
            {
            case 0x00:
                return "Vendor Specific";
            case 0x01:
                return "UFSHCI";
            case 0xFF:
                return "Universal Flash Storage Controller";
            default:
                return nullptr;
            }
        case 0xFF:
            return prog_if == 0xFF ? "Mass Storage Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x02:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Ethernet Controller" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "Token Ring Network Controller" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "FDDI Network Controller" : nullptr;
        case 0x03:
            return prog_if == 0xFF ? "ATM Network Controller" : nullptr;
        case 0x04:
            return prog_if == 0xFF ? "ISDN Controller" : nullptr;
        case 0x05:
            return prog_if == 0xFF ? "WorldFip Controller" : nullptr;
        case 0x06:
            return prog_if == 0xFF ? "PICMG Controller" : nullptr;
        case 0x07:
            return prog_if == 0xFF ? "Infiniband Controller" : nullptr;
        case 0x08:
            return prog_if == 0xFF ? "Fabric Controller" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Network Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x03:
        switch (subclass)
        {
        case 0x00:
            switch (prog_if)
            {
            case 0x00:
                return "VGA Controller";
            case 0x01:
                return "8514 Controller";
            case 0xFF:
                return "VGA Compatible Controller";
            default:
                return nullptr;
            }
        case 0x01:
            return prog_if == 0xFF ? "XGA Compatible Controller" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "3D Controller" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Display Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x04:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Multimedia Video Controller" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "Multimedia Audio Controller" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "Computer Telephony Device" : nullptr;
        case 0x03:
            return prog_if == 0xFF ? "Audio Device" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Multimedia Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x05:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "RAM Memory" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "FLASH Memory" : nullptr;
        case 0x02:
            switch (prog_if)
            {
            case 0x00:
                return "CXL Memory Vendor Specific Device";
            case 0x10:
                return "CXL Memory Device (CXL 2.x)";
            case 0xFF:
                return "CXL";
            default:
                return nullptr;
            }
        case 0xFF:
            return prog_if == 0xFF ? "Memory Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x06:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Host Bridge" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "ISA Bridge" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "EISA Bridge" : nullptr;
        case 0x03:
            return prog_if == 0xFF ? "MicroChannel Bridge" : nullptr;
        case 0x04:
            switch (prog_if)
            {
            case 0x00:
                return "Normal Decode";
            case 0x01:
                return "Subtractive Decode";
            case 0xFF:
                return "PCI Bridge";
            default:
                return nullptr;
            }
        case 0x05:
            return prog_if == 0xFF ? "PCMCIA Bridge" : nullptr;
        case 0x06:
            return prog_if == 0xFF ? "NuBus Bridge" : nullptr;
        case 0x07:
            return prog_if == 0xFF ? "CardBus Bridge" : nullptr;
        case 0x08:
            switch (prog_if)
            {
            case 0x00:
                return "Transparent Mode";
            case 0x01:
                return "Endpoint Mode";
            case 0xFF:
                return "RACEway Bridge";
            default:
                return nullptr;
            }
        case 0x09:
            switch (prog_if)
            {
            case 0x40:
                return "Primary Bus Towards Host CPU";
            case 0x80:
                return "Secondary Bus Towards Host CPU";
            case 0xFF:
                return "Semi-Transparent PCI-to-PCI Bridge";
            default:
                return nullptr;
            }
        case 0x0A:
            return prog_if == 0xFF ? "InfiniBand-to-PCI Host Bridge" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Bridge" : nullptr;
        default:
            return nullptr;
        }
    case 0x07:
        switch (subclass)
        {
        case 0x00:
            switch (prog_if)
            {
            case 0x00:
                return "8250";
            case 0x01:
                return "16450";
            case 0x02:
                return "16550";
            case 0x03:
                return "16650";
            case 0x04:
                return "16750";
            case 0x05:
                return "16850";
            case 0x06:
                return "16950";
            case 0xFF:
                return "Serial Controller";
            default:
                return nullptr;
            }
        case 0x01:
            switch (prog_if)
            {
            case 0x00:
                return "SPP";
            case 0x01:
                return "BiDir";
            case 0x02:
                return "ECP";
            case 0x03:
                return "IEEE 1284";
            case 0xFE:
                return "IEEE 1284 Target";
            case 0xFF:
                return "Parallel Controller";
            default:
                return nullptr;
            }
        case 0x02:
            return prog_if == 0xFF ? "Multiport Serial Controller" : nullptr;
        case 0x03:
            switch (prog_if)
            {
            case 0x00:
                return "Generic";
            case 0x01:
                return "Hayes/16450";
            case 0x02:
                return "Hayes/16550";
            case 0x03:
                return "Hayes/16650";
            case 0x04:
                return "Hayes/16750";
            case 0xFF:
                return "Modem";
            default:
                return nullptr;
            }
        case 0x04:
            return prog_if == 0xFF ? "GPIB Controller" : nullptr;
        case 0x05:
            return prog_if == 0xFF ? "Smart Card Controller" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Communication Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x08:
        switch (subclass)
        {
        case 0x00:
            switch (prog_if)
            {
            case 0x00:
                return "8259";
            case 0x01:
                return "ISA PIC";
            case 0x02:
                return "EISA PIC";
            case 0x10:
                return "IO-APIC";
            case 0x20:
                return "IO(X)-APIC";
            case 0xFF:
                return "PIC";
            default:
                return nullptr;
            }
        case 0x01:
            switch (prog_if)
            {
            case 0x00:
                return "8237";
            case 0x01:
                return "ISA DMA";
            case 0x02:
                return "EISA DMA";
            case 0xFF:
                return "DMA Controller";
            default:
                return nullptr;
            }
        case 0x02:
            switch (prog_if)
            {
            case 0x00:
                return "8254";
            case 0x01:
                return "ISA Timer";
            case 0x02:
                return "EISA Timer";
            case 0x03:
                return "HPET";
            case 0xFF:
                return "Timer";
            default:
                return nullptr;
            }
        case 0x03:
            switch (prog_if)
            {
            case 0x00:
                return "Generic";
            case 0x01:
                return "ISA RTC";
            case 0xFF:
                return "RTC";
            default:
                return nullptr;
            }
        case 0x04:
            return prog_if == 0xFF ? "PCI Hot-Plug Controller" : nullptr;
        case 0x05:
            return prog_if == 0xFF ? "SD Host Controller" : nullptr;
        case 0x06:
            return prog_if == 0xFF ? "IOMMU" : nullptr;
        case 0x99:
            switch (prog_if)
            {
            case 0x01:
                return "TAP Timing Card";
            case 0xFF:
                return "Timing Card";
            default:
                return nullptr;
            }
        case 0xFF:
            return prog_if == 0xFF ? "Generic System Peripheral" : nullptr;
        default:
            return nullptr;
        }
    case 0x09:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Keyboard Controller" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "Digitizer Pen" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "Mouse Controller" : nullptr;
        case 0x03:
            return prog_if == 0xFF ? "Scanner Controller" : nullptr;
        case 0x04:
            switch (prog_if)
            {
            case 0x00:
                return "Generic";
            case 0x10:
                return "Extended";
            case 0xFF:
                return "Gameport Controller";
            default:
                return nullptr;
            }
        case 0xFF:
            return prog_if == 0xFF ? "Input Device Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x0A:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Generic Docking Station" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Docking Station" : nullptr;
        default:
            return nullptr;
        }
    case 0x0B:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "386" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "486" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "Pentium" : nullptr;
        case 0x10:
            return prog_if == 0xFF ? "Alpha" : nullptr;
        case 0x20:
            return prog_if == 0xFF ? "PowerPC" : nullptr;
        case 0x30:
            return prog_if == 0xFF ? "MIPS" : nullptr;
        case 0x40:
            return prog_if == 0xFF ? "Co-Processor" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Processor" : nullptr;
        default:
            return nullptr;
        }
    case 0x0C:
        switch (subclass)
        {
        case 0x00:
            switch (prog_if)
            {
            case 0x00:
                return "Generic";
            case 0x10:
                return "OHCI";
            case 0xFF:
                return "FireWire (IEEE 1394)";
            default:
                return nullptr;
            }
        case 0x01:
            return prog_if == 0xFF ? "ACCESS Bus" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "SSA" : nullptr;
        case 0x03:
            switch (prog_if)
            {
            case 0x00:
                return "UHCI";
            case 0x10:
                return "OHCI";
            case 0x20:
                return "EHCI";
            case 0x30:
                return "XHCI";
            case 0x40:
                return "USB4 Host Interface";
            case 0xFE:
                return "USB Device";
            case 0xFF:
                return "USB Controller";
            default:
                return nullptr;
            }
        case 0x04:
            return prog_if == 0xFF ? "Fibre Channel" : nullptr;
        case 0x05:
            return prog_if == 0xFF ? "SMBus" : nullptr;
        case 0x06:
            return prog_if == 0xFF ? "InfiniBand" : nullptr;
        case 0x07:
            switch (prog_if)
            {
            case 0x00:
                return "SMIC";
            case 0x01:
                return "KCS";
            case 0x02:
                return "Block Transfer";
            case 0xFF:
                return "IPMI Interface";
            default:
                return nullptr;
            }
        case 0x08:
            return prog_if == 0xFF ? "SERCOS Interface" : nullptr;
        case 0x09:
            return prog_if == 0xFF ? "CANBUS" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Serial Bus Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x0D:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "IRDA Controller" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "Consumer IR Controller" : nullptr;
        case 0x10:
            return prog_if == 0xFF ? "RF Controller" : nullptr;
        case 0x11:
            return prog_if == 0xFF ? "Bluetooth" : nullptr;
        case 0x12:
            return prog_if == 0xFF ? "Broadband" : nullptr;
        case 0x20:
            return prog_if == 0xFF ? "802.1a Controller" : nullptr;
        case 0x21:
            return prog_if == 0xFF ? "802.1b Controller" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Wireless Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x0E:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "I2O" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Intelligent Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x0F:
        switch (subclass)
        {
        case 0x01:
            return prog_if == 0xFF ? "Satellite TV Controller" : nullptr;
        case 0x02:
            return prog_if == 0xFF ? "Satellite Audio Communication Controller" : nullptr;
        case 0x03:
            return prog_if == 0xFF ? "Satellite Voice Communication Controller" : nullptr;
        case 0x04:
            return prog_if == 0xFF ? "Satellite Data Communication Controller" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Satellite Communication Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x10:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "Network And Computing Encryption Device" : nullptr;
        case 0x10:
            return prog_if == 0xFF ? "Entertainment Encryption Device" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Encryption Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x11:
        switch (subclass)
        {
        case 0x00:
            return prog_if == 0xFF ? "DPIO Module" : nullptr;
        case 0x01:
            return prog_if == 0xFF ? "Performance Counters" : nullptr;
        case 0x10:
            return prog_if == 0xFF ? "Communication Synchronizer" : nullptr;
        case 0x20:
            return prog_if == 0xFF ? "Signal Processing Management" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Signal Processing Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x12:
        switch (subclass)
        {
        case 0x01:
            return prog_if == 0xFF ? "SNIA Smart Data Accelerator Interface (SDXI) Controller" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Processing Accelerator" : nullptr;
        default:
            return nullptr;
        }
    case 0x13:
        return subclass == 0xFF && prog_if == 0xFF ? "Non-Essential Instrumentation" : nullptr;
    case 0x40:
        return subclass == 0xFF && prog_if == 0xFF ? "Co-Processor" : nullptr;
    case 0xFF:
        return subclass == 0xFF && prog_if == 0xFF ? "Unassigned Class" : nullptr;
    default:
        return nullptr;
    }
}

cstr PCI::GetVendorName(u16 vendor_id)
{
    switch (vendor_id)
    {
    case 0x1002:
        return "Advanced Micro Devices, Inc. [AMD/ATI]";
    case 0x1022:
        return "Advanced Micro Devices, Inc. [AMD]";
    case 0x10DE:
        return "NVIDIA Corporation";
    case 0x8086:
        return "Intel Corp.";
    default:
        return nullptr;
    }
}

cstr PCI::GetDeviceName(u16 vendor_id, u16 device_id)
{
    switch (vendor_id)
    {
    case 0x8086:
        switch (device_id)
        {
        case 0x29C0:
            return "82G33/G31/P35/P31 Express DRAM Controller";
        case 0x10D3:
            return "82574L Gigabit Network Connection";
        case 0x2918:
            return "82801IB (ICH9) LPC Interface Controller";
        case 0x2922:
            return "82801IR/IO/IH (ICH9R/DO/DH) 6 port SATA Controller [AHCI mode]";
        case 0x2930:
            return "82801I (ICH9 Family) SMBus Controller";
        default:
            break;
        }
    default:
        return nullptr;
    }
}

void PCI::EnumeratePCI(ACPI::MCFG_Header *mcfg)
{
    mcfg->ForEach(
        [](const ACPI::MCFG_Entry &entry)
        {
            for (auto bus = entry.StartPCIBus; bus < entry.EndPCIBus; ++bus)
                EnumerateBus(entry.BaseAddressLo, bus);
            return false;
        });
}

void PCI::EnumerateBus(uptr base_address, uptr bus)
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto offset = bus << 20;
    auto bus_address = base_address + offset;

    ptm.MapPage(reinterpret_cast<void *>(bus_address), reinterpret_cast<void *>(bus_address));

    auto device_header = reinterpret_cast<Device_Header *>(bus_address);

    if (device_header->DeviceID == 0x0000 || device_header->DeviceID == 0xffff)
        return;

    for (usize device = 0; device < 32; ++device)
        EnumerateDevice(bus_address, device);
}

void PCI::EnumerateDevice(uptr bus_address, uptr device)
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto offset = device << 15;
    auto device_address = bus_address + offset;

    ptm.MapPage(reinterpret_cast<void *>(device_address), reinterpret_cast<void *>(device_address));

    auto device_header = reinterpret_cast<Device_Header *>(device_address);

    if (device_header->DeviceID == 0x0000 || device_header->DeviceID == 0xffff)
        return;

    for (usize function = 0; function < 8; ++function)
        EnumerateFunction(device_address, function);
}

void PCI::EnumerateFunction(uptr device_address, uptr function)
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto offset = function << 12;
    auto function_address = device_address + offset;

    ptm.MapPage(reinterpret_cast<void *>(function_address), reinterpret_cast<void *>(function_address));

    auto device_header = reinterpret_cast<Device_Header *>(function_address);

    if (device_header->DeviceID == 0x0000 || device_header->DeviceID == 0xffff)
        return;

    auto class_code_desc = GetDeviceDescriptor(device_header->ClassCode);
    auto subclass_desc = GetDeviceDescriptor(device_header->ClassCode, device_header->Subclass);
    auto prog_if_desc = GetDeviceDescriptor(device_header->ClassCode, device_header->Subclass, device_header->ProgIF);

    auto device_name = GetDeviceName(device_header->VendorID, device_header->DeviceID);
    auto vendor_name = GetVendorName(device_header->VendorID);

    printf("%04x:%04x %02x:%02x:%02x", device_header->VendorID, device_header->DeviceID, device_header->ClassCode, device_header->Subclass, device_header->ProgIF);
    printf(" - %s, %s", device_name, vendor_name);
    if (class_code_desc)
    {
        if (subclass_desc)
        {
            if (prog_if_desc)
                printf(" [ %s, %s, %s ]", class_code_desc, subclass_desc, prog_if_desc);
            else
                printf(" [ %s, %s ]", class_code_desc, subclass_desc);
        }
        else
            printf(" [ %s ]", class_code_desc);
    }
    print("\r\n");
}
