#include <scriptos/pci.h>
#include <scriptos/types.h>

cstr pci::GetDeviceDescriptor(
    u8 base_class,
    u8 sub_class,
    u8 prog_if)
{
    switch (base_class)
    {
    case 0x00:
        switch (sub_class)
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
        switch (sub_class)
        {
        case 0x00:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "SCSI Storage Controller";
            default:
                return nullptr;
            }
        case 0x01:
            switch (prog_if)
            {
            case 0x00:
                return "ISA Compatibility Mode-Only Controller";
            case 0x05:
                return "PCI Native Mode-Only Controller";
            case 0x0A:
                return "ISA Compatibility Mode Controller "
                       "with Dual-Channel PCI Native Mode Support";
            case 0x0F:
                return "PCI Native Mode Controller "
                       "with Dual-Channel ISA Compatibility Mode Support";
            case 0x80:
                return "ISA Compatibility Mode-Only Controller "
                       "with Bus Mastering";
            case 0x85:
                return "PCI Native Mode-Only Controller "
                       "with Bus Mastering";
            case 0x8A:
                return "ISA Compatibility Mode Controller "
                       "with Dual-Channel PCI Native Mode Support "
                       "and Bus Mastering";
            case 0x8F:
                return "PCI Native Mode Controller "
                       "with Dual-Channel ISA Compatibility Mode Support "
                       "and Bus Mastering";
            case 0xFF:
                return "IDE Interface";
            default:
                return nullptr;
            }
        case 0x02:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "Floppy Disk Controller";
            default:
                return nullptr;
            }
        case 0x03:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "IPI Bus Controller";
            default:
                return nullptr;
            }
        case 0x04:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "RAID Bus Controller";
            default:
                return nullptr;
            }
        case 0x05:
            switch (prog_if)
            {
            case 0x20:
                return "ADMA Single Stepping ATA Controller";
            case 0x30:
                return "ADMA Continuous Operation ATA Controller";
            case 0xFF:
                return "ATA Controller";
            default:
                return nullptr;
            }
        case 0x06:
            switch (prog_if)
            {
            case 0x00:
                return "Vendor Specific SATA Controller";
            case 0x01:
                return "AHCI 1.0 Controller";
            case 0x02:
                return "Serial Storage Bus Controller";
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
                return "NVMHCI Controller";
            case 0x02:
                return "NVM Express Controller";
            case 0xFF:
                return "Non-Volatile Memory Controller";
            default:
                return nullptr;
            }
        case 0x09:
            switch (prog_if)
            {
            case 0x00:
                return "Vendor Specific Flash Controller";
            case 0x01:
                return "UFSHCI Controller";
            case 0xFF:
                return "Universal Flash Storage Controller";
            default:
                return nullptr;
            }
        case 0xFF:
            switch (prog_if)
            {
            case 0xFF:
                return "Mass Storage Controller";
            default:
                return nullptr;
            }
        default:
            return nullptr;
        }
    case 0x02:
        switch (sub_class)
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
            switch (prog_if)
            {
            case 0xFF:
                return "Network Controller";
            default:
                return nullptr;
            }
        default:
            return nullptr;
        }
    case 0x03:
        switch (sub_class)
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
        switch (sub_class)
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
        switch (sub_class)
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
        switch (sub_class)
        {
        case 0x00:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "Host Bridge";
            default:
                return nullptr;
            }
        case 0x01:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "ISA Bridge";
            default:
                return nullptr;
            }
        case 0x02:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "EISA Bridge";
            default:
                return nullptr;
            }
        case 0x03:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "MCA Bridge";
            default:
                return nullptr;
            }
        case 0x04:
            switch (prog_if)
            {
            case 0x00:
                return "PCI-to-PCI Bridge (Normal)";
            case 0x01:
                return "PCI-to-PCI Bridge (Subtractive)";
            case 0xFF:
                return "PCI-to-PCI Bridge";
            default:
                return nullptr;
            }
        case 0x05:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "PCMCIA Bridge";
            default:
                return nullptr;
            }
        case 0x06:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "NuBus Bridge";
            default:
                return nullptr;
            }
        case 0x07:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "CardBus Bridge";
            default:
                return nullptr;
            }
        case 0x08:
            switch (prog_if)
            {
            case 0x00:
                return "RACEway Bridge (Transparent)";
            case 0x01:
                return "RACEway Bridge (Endpoint)";
            case 0xFF:
                return "RACEway Bridge";
            default:
                return nullptr;
            }
        case 0x09:
            switch (prog_if)
            {
            case 0x40:
                return "Semi-Transparent PCI-to-PCI Bridge (Primary Bus "
                       "Towards Host)";
            case 0x80:
                return "Semi-Transparent PCI-to-PCI Bridge (Secondary Bus "
                       "Towards Host)";
            case 0xFF:
                return "Semi-Transparent PCI-to-PCI Bridge";
            default:
                return nullptr;
            }
        case 0x0A:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "InfiniBand-to-PCI Host Bridge";
            default:
                return nullptr;
            }
        case 0x0B:
            switch (prog_if)
            {
            case 0x00:
                return "Advanced Switching to PCI Host Bridge "
                       "(Custom Interface)";
            case 0x01:
                return "Advanced Switching to PCI Host Bridge "
                       "(ASI-SIG Defined Portal Interface)";
            case 0xFF:
                return "Advanced Switching to PCI Host Bridge";
            default:
                return nullptr;
            }
        case 0x80:
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "Other Bridge Device";
            default:
                return nullptr;
            }
        case 0xFF:
            switch (prog_if)
            {
            case 0xFF:
                return "Bridge Device";
            default:
                return nullptr;
            }
        default:
            return nullptr;
        }
    case 0x07:
        switch (sub_class)
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
        switch (sub_class)
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
        switch (sub_class)
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
        switch (sub_class)
        {
        case 0x00:
            return prog_if == 0xFF ? "Generic Docking Station" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Docking Station" : nullptr;
        default:
            return nullptr;
        }
    case 0x0B:
        switch (sub_class)
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
        switch (sub_class)
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
            switch (prog_if)
            {
            case 0x00:
            case 0xFF:
                return "SMBus";
            default:
                return nullptr;
            }
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
        switch (sub_class)
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
        switch (sub_class)
        {
        case 0x00:
            return prog_if == 0xFF ? "I2O" : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Intelligent Controller" : nullptr;
        default:
            return nullptr;
        }
    case 0x0F:
        switch (sub_class)
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
        switch (sub_class)
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
        switch (sub_class)
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
        switch (sub_class)
        {
        case 0x01:
            return prog_if == 0xFF ? "SNIA Smart Data Accelerator Interface "
                                     "(SDXI) Controller"
                                   : nullptr;
        case 0xFF:
            return prog_if == 0xFF ? "Processing Accelerator" : nullptr;
        default:
            return nullptr;
        }
    case 0x13:
        return sub_class == 0xFF && prog_if == 0xFF ? "Non-Essential "
                                                      "Instrumentation"
                                                    : nullptr;
    case 0x40:
        return sub_class == 0xFF && prog_if == 0xFF ? "Co-Processor" : nullptr;
    case 0xFF:
        return sub_class == 0xFF && prog_if == 0xFF ? "Unassigned Class" : nullptr;
    default:
        return nullptr;
    }
}

cstr pci::GetVendorName(u16 vendor_id)
{
    switch (vendor_id)
    {
    case 0x1002:
        return "Advanced Micro Devices, Inc. [AMD/ATI]";
    case 0x1022:
        return "Advanced Micro Devices, Inc. [AMD]";
    case 0x10DE:
        return "NVIDIA Corporation";
    case 0x10EC:
        return "Realtek Semiconductor Co., Ltd.";
    case 0x1234:
        return "QEMU";
    case 0x8086:
        return "Intel Corp.";
    default:
        return nullptr;
    }
}

cstr pci::GetDeviceName(
    u16 vendor_id,
    u16 device_id)
{
    switch (vendor_id)
    {
    case 0x8086:
        switch (device_id)
        {
        case 0x10D3:
            return "82574L Gigabit Network Connection";
        case 0x2448:
            return "82801 Mobile PCI Bridge";
        case 0x2815:
            return "82801HM (ICH8M) LPC Interface Controller";
        case 0x2828:
            return "82801HM/HEM (ICH8M/ICH8M-E) SATA Controller [IDE mode]";
        case 0x2830:
            return "82801H (ICH8 Family) USB UHCI Controller #1";
        case 0x2831:
            return "82801H (ICH8 Family) USB UHCI Controller #2";
        case 0x2832:
            return "82801H (ICH8 Family) USB UHCI Controller #3";
        case 0x2833:
            return "82801H (ICH8 Family) USB UHCI Controller #4";
        case 0x2834:
            return "82801H (ICH8 Family) USB UHCI Controller #4";
        case 0x2835:
            return "82801H (ICH8 Family) USB UHCI Controller #5";
        case 0x2836:
            return "82801H (ICH8 Family) USB2 EHCI Controller #1";
        case 0x283A:
            return "82801H (ICH8 Family) USB2 EHCI Controller #2";
        case 0x283E:
            return "82801H (ICH8 Family) SMBus Controller";
        case 0x283F:
            return "82801H (ICH8 Family) PCI Express Port 1";
        case 0x2841:
            return "82801H (ICH8 Family) PCI Express Port 2";
        case 0x2843:
            return "82801H (ICH8 Family) PCI Express Port 3";
        case 0x2845:
            return "82801H (ICH8 Family) PCI Express Port 4";
        case 0x2847:
            return "82801H (ICH8 Family) PCI Express Port 5";
        case 0x2849:
            return "82801H (ICH8 Family) PCI Express Port 6";
        case 0x284B:
            return "82801H (ICH8 Family) HD Audio Controller";
        case 0x284F:
            return "82801H (ICH8 Family) Thermal Reporting Device";
        case 0x2918:
            return "82801IB (ICH9) LPC Interface Controller";
        case 0x2922:
            return "82801IR/IO/IH (ICH9R/DO/DH) 6 port SATA Controller [AHCI "
                   "mode]";
        case 0x2930:
            return "82801I (ICH9 Family) SMBus Controller";
        case 0x29C0:
            return "82G33/G31/P35/P31 Express DRAM Controller";
        case 0x2A00:
            return "Mobile PM965/GM965/GL960 Memory Controller Hub";
        case 0x2A01:
            return "Mobile PM965/GM965/GL960 PCI Express Root Port";
        case 0x4229:
            return "PRO/Wireless 4965 AG or AGN [Kedron] Network Connection";
        default:
            return nullptr;
        }
    case 0x10DE:
        switch (device_id)
        {
        case 0x060C:
            return "G92M [GeForce 8800M GTX]";
        default:
            return nullptr;
        }
    case 0x10EC:
        switch (device_id)
        {
        case 0x8161:
            return "RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller";
        case 0x8167:
            return "RTL-8110SC/8169SC Gigabit Ethernet";
        case 0x8168:
            return "RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller";
        case 0x8169:
            return "RTL8169 PCI Gigabit Ethernet Controller";
        case 0x816A:
            return "RTL8111xP UART #1";
        case 0x816B:
            return "RTL8111xP UART #2";
        case 0x816C:
            return "RTL8111xP IPMI interface";
        case 0x816D:
            return "RTL811x EHCI host controller";
        case 0x816E:
            return "Realtek RealManage BMC";
        default:
            return nullptr;
        }
    case 0x1234:
        switch (device_id)
        {
        case 0x1111:
            return "Generic VGA Controller";
        default:
            return nullptr;
        }
    default:
        return nullptr;
    }
}
