#include <scriptos/pci.h>
#include <scriptos/types.h>

constexpr u32 pci_key(
    u8 base_class,
    u8 sub_class,
    u8 prog_if)
{
    return (u32(base_class) << 16u) | (u32(sub_class) << 8u) | u32(prog_if);
}

template<typename K, typename V>
struct key_val_t
{
    K key;
    V val;
};

constexpr key_val_t<u32, cstr> pci_device_table[]{

    /* 0x00 */

    { pci_key(0x00, 0x00, 0xFF),                                                        "Non-VGA Compatible Unclassified Device" },

    { pci_key(0x00, 0x01, 0xFF),                                                            "VGA Compatible Unclassified Device" },

    { pci_key(0x00, 0x05, 0xFF),                                                                            "Image Co-Processor" },

    { pci_key(0x00, 0xFF, 0xFF),                                                                           "Unclassified Device" },

    /* 0x01 */

    { pci_key(0x01, 0x00, 0x00),                                                                       "SCSI Storage Controller" },
    { pci_key(0x01, 0x00, 0xFF),                                                                       "SCSI Storage Controller" },

    { pci_key(0x01, 0x01, 0x00),                                                        "ISA Compatibility Mode-Only Controller" },
    { pci_key(0x01, 0x01, 0x05),                                                               "PCI Native Mode-Only Controller" },
    { pci_key(0x01, 0x01, 0x0A),                   "ISA Compatibility Mode Controller with Dual-Channel PCI Native Mode Support" },
    { pci_key(0x01, 0x01, 0x0F),                   "PCI Native Mode Controller with Dual-Channel ISA Compatibility Mode Support" },
    { pci_key(0x01, 0x01, 0x80),                                     "ISA Compatibility Mode-Only Controller with Bus Mastering" },
    { pci_key(0x01, 0x01, 0x85),                                            "PCI Native Mode-Only Controller with Bus Mastering" },
    { pci_key(0x01, 0x01, 0x8A), "ISA Compatibility Mode Controller with Dual-Channel PCI Native Mode Support and Bus Mastering" },
    { pci_key(0x01, 0x01, 0x8F), "PCI Native Mode Controller with Dual-Channel ISA Compatibility Mode Support and Bus Mastering" },
    { pci_key(0x01, 0x01, 0xFF),                                                                                 "IDE Interface" },

    { pci_key(0x01, 0x02, 0x00),                                                                        "Floppy Disk Controller" },
    { pci_key(0x01, 0x02, 0xFF),                                                                        "Floppy Disk Controller" },

    { pci_key(0x01, 0x03, 0x00),                                                                            "IPI Bus Controller" },
    { pci_key(0x01, 0x03, 0xFF),                                                                            "IPI Bus Controller" },

    { pci_key(0x01, 0x04, 0x00),                                                                           "RAID Bus Controller" },
    { pci_key(0x01, 0x04, 0xFF),                                                                           "RAID Bus Controller" },

    { pci_key(0x01, 0x05, 0x20),                                                           "ADMA Single Stepping ATA Controller" },
    { pci_key(0x01, 0x05, 0x30),                                                      "ADMA Continuous Operation ATA Controller" },
    { pci_key(0x01, 0x05, 0xFF),                                                                                "ATA Controller" },

    { pci_key(0x01, 0x06, 0x00),                                                               "Vendor Specific SATA Controller" },
    { pci_key(0x01, 0x06, 0x01),                                                                           "AHCI 1.0 Controller" },
    { pci_key(0x01, 0x06, 0x02),                                                                 "Serial Storage Bus Controller" },
    { pci_key(0x01, 0x06, 0xFF),                                                                               "SATA Controller" },

    { pci_key(0x01, 0x07, 0x01),                                                                            "Serial Storage Bus" },
    { pci_key(0x01, 0x07, 0xFF),                                                               "Serial Attached SCSI Controller" },

    { pci_key(0x01, 0x08, 0x01),                                                                             "NVMHCI Controller" },
    { pci_key(0x01, 0x08, 0x02),                                                                        "NVM Express Controller" },
    { pci_key(0x01, 0x08, 0xFF),                                                                "Non-Volatile Memory Controller" },

    { pci_key(0x01, 0x09, 0x00),                                                              "Vendor Specific Flash Controller" },
    { pci_key(0x01, 0x09, 0x01),                                                                             "UFSHCI Controller" },
    { pci_key(0x01, 0x09, 0xFF),                                                            "Universal Flash Storage Controller" },

    { pci_key(0x01, 0xFF, 0xFF),                                                                       "Mass Storage Controller" },

    /* 0x02 */

    { pci_key(0x02, 0x00, 0xFF),                                                                           "Ethernet Controller" },

    { pci_key(0x02, 0x01, 0xFF),                                                                 "Token Ring Network Controller" },

    { pci_key(0x02, 0x02, 0xFF),                                                                       "FDDI Network Controller" },

    { pci_key(0x02, 0x03, 0xFF),                                                                        "ATM Network Controller" },

    { pci_key(0x02, 0x04, 0xFF),                                                                               "ISDN Controller" },

    { pci_key(0x02, 0x05, 0xFF),                                                                           "WorldFip Controller" },

    { pci_key(0x02, 0x06, 0xFF),                                                                              "PICMG Controller" },

    { pci_key(0x02, 0x07, 0xFF),                                                                         "Infiniband Controller" },

    { pci_key(0x02, 0x08, 0xFF),                                                                             "Fabric Controller" },

    { pci_key(0x02, 0xFF, 0xFF),                                                                            "Network Controller" },

    /* 0x03 */

    { pci_key(0x03, 0x00, 0x00),                                                                                "VGA Controller" },
    { pci_key(0x03, 0x00, 0x01),                                                                               "8514 Controller" },
    { pci_key(0x03, 0x00, 0xFF),                                                                     "VGA Compatible Controller" },

    { pci_key(0x03, 0x01, 0xFF),                                                                     "XGA Compatible Controller" },

    { pci_key(0x03, 0x02, 0xFF),                                                                                 "3D Controller" },

    { pci_key(0x03, 0xFF, 0xFF),                                                                            "Display Controller" },

    /* 0x04 */

    { pci_key(0x04, 0x00, 0xFF),                                                                   "Multimedia Video Controller" },

    { pci_key(0x04, 0x01, 0xFF),                                                                   "Multimedia Audio Controller" },

    { pci_key(0x04, 0x02, 0xFF),                                                                     "Computer Telephony Device" },

    { pci_key(0x04, 0x03, 0xFF),                                                                                  "Audio Device" },

    { pci_key(0x04, 0xFF, 0xFF),                                                                         "Multimedia Controller" },

    /* 0x05 */

    { pci_key(0x05, 0x00, 0xFF),                                                                                    "RAM Memory" },

    { pci_key(0x05, 0x01, 0xFF),                                                                                  "FLASH Memory" },

    { pci_key(0x05, 0x02, 0x00),                                                             "CXL Memory Vendor Specific Device" },
    { pci_key(0x05, 0x02, 0x10),                                                                   "CXL Memory Device (CXL 2.x)" },
    { pci_key(0x05, 0x02, 0xFF),                                                                                           "CXL" },

    { pci_key(0x05, 0xFF, 0xFF),                                                                             "Memory Controller" },

    /* 0x06 */

    { pci_key(0x06, 0x00, 0x00),                                                                                   "Host Bridge" },
    { pci_key(0x06, 0x00, 0xFF),                                                                                   "Host Bridge" },

    { pci_key(0x06, 0x01, 0x00),                                                                                    "ISA Bridge" },
    { pci_key(0x06, 0x01, 0xFF),                                                                                    "ISA Bridge" },

    { pci_key(0x06, 0x02, 0x00),                                                                                   "EISA Bridge" },
    { pci_key(0x06, 0x02, 0xFF),                                                                                   "EISA Bridge" },

    { pci_key(0x06, 0x03, 0x00),                                                                                    "MCA Bridge" },
    { pci_key(0x06, 0x03, 0xFF),                                                                                    "MCA Bridge" },

    { pci_key(0x06, 0x04, 0x00),                                                                    "PCI-to-PCI Bridge (Normal)" },
    { pci_key(0x06, 0x04, 0x01),                                                               "PCI-to-PCI Bridge (Subtractive)" },
    { pci_key(0x06, 0x04, 0xFF),                                                                             "PCI-to-PCI Bridge" },

    { pci_key(0x06, 0x05, 0x00),                                                                                 "PCMCIA Bridge" },
    { pci_key(0x06, 0x05, 0xFF),                                                                                 "PCMCIA Bridge" },

    { pci_key(0x06, 0x06, 0x00),                                                                                  "NuBus Bridge" },
    { pci_key(0x06, 0x06, 0xFF),                                                                                  "NuBus Bridge" },

    { pci_key(0x06, 0x07, 0x00),                                                                                "CardBus Bridge" },
    { pci_key(0x06, 0x07, 0xFF),                                                                                "CardBus Bridge" },

    { pci_key(0x06, 0x08, 0x00),                                                                  "RACEway Bridge (Transparent)" },
    { pci_key(0x06, 0x08, 0x01),                                                                     "RACEway Bridge (Endpoint)" },
    { pci_key(0x06, 0x08, 0xFF),                                                                                "RACEway Bridge" },

    { pci_key(0x06, 0x09, 0x40),                                 "Semi-Transparent PCI-to-PCI Bridge (Primary Bus Towards Host)" },
    { pci_key(0x06, 0x09, 0x80),                               "Semi-Transparent PCI-to-PCI Bridge (Secondary Bus Towards Host)" },
    { pci_key(0x06, 0x09, 0xFF),                                                            "Semi-Transparent PCI-to-PCI Bridge" },

    { pci_key(0x06, 0x0A, 0x00),                                                                 "InfiniBand-to-PCI Host Bridge" },
    { pci_key(0x06, 0x0A, 0xFF),                                                                 "InfiniBand-to-PCI Host Bridge" },

    { pci_key(0x06, 0x0B, 0x00),                                      "Advanced Switching to PCI Host Bridge (Custom Interface)" },
    { pci_key(0x06, 0x0B, 0x01),                      "Advanced Switching to PCI Host Bridge (ASI-SIG Defined Portal Interface)" },
    { pci_key(0x06, 0x0B, 0xFF),                                                         "Advanced Switching to PCI Host Bridge" },

    { pci_key(0x06, 0x80, 0x00),                                                                           "Other Bridge Device" },
    { pci_key(0x06, 0x80, 0xFF),                                                                           "Other Bridge Device" },

    { pci_key(0x06, 0xFF, 0xFF),                                                                                 "Bridge Device" },

    /* 0x07 */

    { pci_key(0x07, 0x00, 0x00),                                                                                          "8250" },
    { pci_key(0x07, 0x00, 0x01),                                                                                         "16450" },
    { pci_key(0x07, 0x00, 0x02),                                                                                         "16550" },
    { pci_key(0x07, 0x00, 0x03),                                                                                         "16650" },
    { pci_key(0x07, 0x00, 0x04),                                                                                         "16750" },
    { pci_key(0x07, 0x00, 0x05),                                                                                         "16850" },
    { pci_key(0x07, 0x00, 0x06),                                                                                         "16950" },
    { pci_key(0x07, 0x00, 0xFF),                                                                             "Serial Controller" },

    { pci_key(0x07, 0x01, 0x00),                                                                                           "SPP" },
    { pci_key(0x07, 0x01, 0x01),                                                                                         "BiDir" },
    { pci_key(0x07, 0x01, 0x02),                                                                                           "ECP" },
    { pci_key(0x07, 0x01, 0x03),                                                                                     "IEEE 1284" },
    { pci_key(0x07, 0x01, 0xFE),                                                                              "IEEE 1284 Target" },
    { pci_key(0x07, 0x01, 0xFF),                                                                           "Parallel Controller" },

    { pci_key(0x07, 0x02, 0xFF),                                                                   "Multiport Serial Controller" },

    { pci_key(0x07, 0x03, 0x00),                                                                                       "Generic" },
    { pci_key(0x07, 0x03, 0x01),                                                                                   "Hayes/16450" },
    { pci_key(0x07, 0x03, 0x02),                                                                                   "Hayes/16550" },
    { pci_key(0x07, 0x03, 0x03),                                                                                   "Hayes/16650" },
    { pci_key(0x07, 0x03, 0x04),                                                                                   "Hayes/16750" },
    { pci_key(0x07, 0x03, 0xFF),                                                                                         "Modem" },

    { pci_key(0x07, 0x04, 0xFF),                                                                               "GPIB Controller" },

    { pci_key(0x07, 0x05, 0xFF),                                                                         "Smart Card Controller" },

    { pci_key(0x07, 0xFF, 0xFF),                                                                      "Communication Controller" },

    /* 0x08 */

    { pci_key(0x08, 0x00, 0x00),                                                                                          "8259" },
    { pci_key(0x08, 0x00, 0x01),                                                                                       "ISA PIC" },
    { pci_key(0x08, 0x00, 0x02),                                                                                      "EISA PIC" },
    { pci_key(0x08, 0x00, 0x10),                                                                                       "IO-APIC" },
    { pci_key(0x08, 0x00, 0x20),                                                                                    "IO(X)-APIC" },
    { pci_key(0x08, 0x00, 0xFF),                                                                                           "PIC" },

    { pci_key(0x08, 0x01, 0x00),                                                                                          "8237" },
    { pci_key(0x08, 0x01, 0x01),                                                                                       "ISA DMA" },
    { pci_key(0x08, 0x01, 0x02),                                                                                      "EISA DMA" },
    { pci_key(0x08, 0x01, 0xFF),                                                                                "DMA Controller" },

    { pci_key(0x08, 0x02, 0x00),                                                                                          "8254" },
    { pci_key(0x08, 0x02, 0x01),                                                                                     "ISA Timer" },
    { pci_key(0x08, 0x02, 0x02),                                                                                    "EISA Timer" },
    { pci_key(0x08, 0x02, 0x03),                                                                                          "HPET" },
    { pci_key(0x08, 0x02, 0xFF),                                                                                         "Timer" },

    { pci_key(0x08, 0x03, 0x00),                                                                                       "Generic" },
    { pci_key(0x08, 0x03, 0x01),                                                                                       "ISA RTC" },
    { pci_key(0x08, 0x03, 0xFF),                                                                                           "RTC" },

    { pci_key(0x08, 0x04, 0xFF),                                                                       "PCI Hot-Plug Controller" },

    { pci_key(0x08, 0x05, 0xFF),                                                                            "SD Host Controller" },

    { pci_key(0x08, 0x06, 0xFF),                                                                                         "IOMMU" },

    { pci_key(0x08, 0x99, 0x01),                                                                               "TAP Timing Card" },
    { pci_key(0x08, 0x99, 0xFF),                                                                                   "Timing Card" },

    { pci_key(0x08, 0xFF, 0xFF),                                                                     "Generic System Peripheral" },

    /* 0x09 */

    { pci_key(0x09, 0x00, 0xFF),                                                                           "Keyboard Controller" },

    { pci_key(0x09, 0x01, 0xFF),                                                                                 "Digitizer Pen" },

    { pci_key(0x09, 0x02, 0xFF),                                                                              "Mouse Controller" },

    { pci_key(0x09, 0x03, 0xFF),                                                                            "Scanner Controller" },

    { pci_key(0x09, 0x04, 0x00),                                                                                       "Generic" },
    { pci_key(0x09, 0x04, 0x10),                                                                                      "Extended" },
    { pci_key(0x09, 0x04, 0xFF),                                                                           "Gameport Controller" },

    { pci_key(0x09, 0xFF, 0xFF),                                                                       "Input Device Controller" },

    /* 0x0A */

    { pci_key(0x0A, 0x00, 0xFF),                                                                       "Generic Docking Station" },

    { pci_key(0x0A, 0xFF, 0xFF),                                                                               "Docking Station" },

    /* 0x0B */

    { pci_key(0x0B, 0x00, 0xFF),                                                                                           "386" },

    { pci_key(0x0B, 0x01, 0xFF),                                                                                           "486" },

    { pci_key(0x0B, 0x02, 0xFF),                                                                                       "Pentium" },

    { pci_key(0x0B, 0x10, 0xFF),                                                                                         "Alpha" },

    { pci_key(0x0B, 0x20, 0xFF),                                                                                       "PowerPC" },

    { pci_key(0x0B, 0x30, 0xFF),                                                                                          "MIPS" },

    { pci_key(0x0B, 0x40, 0xFF),                                                                                  "Co-Processor" },

    { pci_key(0x0B, 0xFF, 0xFF),                                                                                     "Processor" },

    /* 0x0C */

    { pci_key(0x0C, 0x00, 0x00),                                                                                       "Generic" },
    { pci_key(0x0C, 0x00, 0x10),                                                                                          "OHCI" },
    { pci_key(0x0C, 0x00, 0xFF),                                                                          "FireWire (IEEE 1394)" },

    { pci_key(0x0C, 0x01, 0xFF),                                                                                    "ACCESS Bus" },

    { pci_key(0x0C, 0x02, 0xFF),                                                                                           "SSA" },

    { pci_key(0x0C, 0x03, 0x00),                                                                                          "UHCI" },
    { pci_key(0x0C, 0x03, 0x10),                                                                                          "OHCI" },
    { pci_key(0x0C, 0x03, 0x20),                                                                                          "EHCI" },
    { pci_key(0x0C, 0x03, 0x30),                                                                                          "XHCI" },
    { pci_key(0x0C, 0x03, 0x40),                                                                           "USB4 Host Interface" },
    { pci_key(0x0C, 0x03, 0xFE),                                                                                    "USB Device" },
    { pci_key(0x0C, 0x03, 0xFF),                                                                                "USB Controller" },

    { pci_key(0x0C, 0x04, 0xFF),                                                                                 "Fibre Channel" },

    { pci_key(0x0C, 0x05, 0x00),                                                                                         "SMBus" },
    { pci_key(0x0C, 0x05, 0xFF),                                                                                         "SMBus" },

    { pci_key(0x0C, 0x06, 0xFF),                                                                                    "InfiniBand" },

    { pci_key(0x0C, 0x07, 0x00),                                                                                          "SMIC" },
    { pci_key(0x0C, 0x07, 0x01),                                                                                           "KCS" },
    { pci_key(0x0C, 0x07, 0x02),                                                                                "Block Transfer" },
    { pci_key(0x0C, 0x07, 0xFF),                                                                                "IPMI Interface" },

    { pci_key(0x0C, 0x08, 0xFF),                                                                              "SERCOS Interface" },
    { pci_key(0x0C, 0x09, 0xFF),                                                                                        "CANBUS" },
    { pci_key(0x0C, 0xFF, 0xFF),                                                                         "Serial Bus Controller" },

    /* 0x0D */

    { pci_key(0x0D, 0x00, 0xFF),                                                                               "IRDA Controller" },

    { pci_key(0x0D, 0x01, 0xFF),                                                                        "Consumer IR Controller" },

    { pci_key(0x0D, 0x10, 0xFF),                                                                                 "RF Controller" },

    { pci_key(0x0D, 0x11, 0xFF),                                                                                     "Bluetooth" },

    { pci_key(0x0D, 0x12, 0xFF),                                                                                     "Broadband" },

    { pci_key(0x0D, 0x20, 0xFF),                                                                             "802.1a Controller" },

    { pci_key(0x0D, 0x21, 0xFF),                                                                             "802.1b Controller" },

    { pci_key(0x0D, 0xFF, 0xFF),                                                                           "Wireless Controller" },

    /* 0x0E */

    { pci_key(0x0E, 0x00, 0xFF),                                                                                           "I2O" },

    { pci_key(0x0E, 0xFF, 0xFF),                                                                        "Intelligent Controller" },

    /* 0x0F */

    { pci_key(0x0F, 0x01, 0xFF),                                                                       "Satellite TV Controller" },

    { pci_key(0x0F, 0x02, 0xFF),                                                      "Satellite Audio Communication Controller" },

    { pci_key(0x0F, 0x03, 0xFF),                                                      "Satellite Voice Communication Controller" },

    { pci_key(0x0F, 0x04, 0xFF),                                                       "Satellite Data Communication Controller" },

    { pci_key(0x0F, 0xFF, 0xFF),                                                            "Satellite Communication Controller" },

    /* 0x10 */

    { pci_key(0x10, 0x00, 0xFF),                                                       "Network And Computing Encryption Device" },

    { pci_key(0x10, 0x10, 0xFF),                                                               "Entertainment Encryption Device" },

    { pci_key(0x10, 0xFF, 0xFF),                                                                         "Encryption Controller" },

    /* 0x11 */

    { pci_key(0x11, 0x00, 0xFF),                                                                                   "DPIO Module" },

    { pci_key(0x11, 0x01, 0xFF),                                                                          "Performance Counters" },

    { pci_key(0x11, 0x10, 0xFF),                                                                    "Communication Synchronizer" },

    { pci_key(0x11, 0x20, 0xFF),                                                                  "Signal Processing Management" },

    { pci_key(0x11, 0xFF, 0xFF),                                                                  "Signal Processing Controller" },

    /* 0x12 */

    { pci_key(0x12, 0x01, 0xFF),                                       "SNIA Smart Data Accelerator Interface (SDXI) Controller" },

    { pci_key(0x12, 0xFF, 0xFF),                                                                        "Processing Accelerator" },

    /* 0x13 */

    { pci_key(0x13, 0xFF, 0xFF),                                                                 "Non-Essential Instrumentation" },

    /* 0x40 */

    { pci_key(0x40, 0xFF, 0xFF),                                                                                  "Co-Processor" },

    /* 0xFF */

    { pci_key(0xFF, 0xFF, 0xFF),                                                                              "Unassigned Class" },
};

cstr kernel::GetPciDeviceDescriptor(
    u8 base_class,
    u8 sub_class,
    u8 prog_if)
{
    auto key = pci_key(base_class, sub_class, prog_if);
    for (auto& entry : pci_device_table)
        if (entry.key == key)
            return entry.val;
    return nullptr;
}

constexpr key_val_t<u16, cstr> pci_vendor_table[]{
    { 0x1002, "Advanced Micro Devices, Inc. [AMD/ATI]" },
    { 0x1022,     "Advanced Micro Devices, Inc. [AMD]" },
    { 0x10DE,                     "NVIDIA Corporation" },
    { 0x10EC,        "Realtek Semiconductor Co., Ltd." },
    { 0x1234,                                   "QEMU" },
    { 0x8086,                            "Intel Corp." },
};

cstr kernel::GetPciVendorName(u16 vendor_id)
{
    for (auto& entry : pci_vendor_table)
        if (entry.key == vendor_id)
            return entry.val;
    return nullptr;
}

constexpr u32 pci_vendor_device_key(
    u16 vendor_id,
    u16 device_id)
{
    return (u32(vendor_id) << 16u) | u32(device_id);
}

constexpr key_val_t<u32, cstr> pci_vendor_device_table[]{
    { pci_vendor_device_key(0x8086, 0x10D3),                              "82574L Gigabit Network Connection" },
    { pci_vendor_device_key(0x8086, 0x2448),                                        "82801 Mobile PCI Bridge" },
    { pci_vendor_device_key(0x8086, 0x2815),                       "82801HM (ICH8M) LPC Interface Controller" },
    { pci_vendor_device_key(0x8086, 0x2828),         "82801HM/HEM (ICH8M/ICH8M-E) SATA Controller [IDE mode]" },
    { pci_vendor_device_key(0x8086, 0x2830),                    "82801H (ICH8 Family) USB UHCI Controller #1" },
    { pci_vendor_device_key(0x8086, 0x2831),                    "82801H (ICH8 Family) USB UHCI Controller #2" },
    { pci_vendor_device_key(0x8086, 0x2832),                    "82801H (ICH8 Family) USB UHCI Controller #3" },
    { pci_vendor_device_key(0x8086, 0x2833),                    "82801H (ICH8 Family) USB UHCI Controller #4" },
    { pci_vendor_device_key(0x8086, 0x2834),                    "82801H (ICH8 Family) USB UHCI Controller #4" },
    { pci_vendor_device_key(0x8086, 0x2835),                    "82801H (ICH8 Family) USB UHCI Controller #5" },
    { pci_vendor_device_key(0x8086, 0x2836),                   "82801H (ICH8 Family) USB2 EHCI Controller #1" },
    { pci_vendor_device_key(0x8086, 0x283A),                   "82801H (ICH8 Family) USB2 EHCI Controller #2" },
    { pci_vendor_device_key(0x8086, 0x283E),                          "82801H (ICH8 Family) SMBus Controller" },
    { pci_vendor_device_key(0x8086, 0x283F),                        "82801H (ICH8 Family) PCI Express Port 1" },
    { pci_vendor_device_key(0x8086, 0x2841),                        "82801H (ICH8 Family) PCI Express Port 2" },
    { pci_vendor_device_key(0x8086, 0x2843),                        "82801H (ICH8 Family) PCI Express Port 3" },
    { pci_vendor_device_key(0x8086, 0x2845),                        "82801H (ICH8 Family) PCI Express Port 4" },
    { pci_vendor_device_key(0x8086, 0x2847),                        "82801H (ICH8 Family) PCI Express Port 5" },
    { pci_vendor_device_key(0x8086, 0x2849),                        "82801H (ICH8 Family) PCI Express Port 6" },
    { pci_vendor_device_key(0x8086, 0x284B),                       "82801H (ICH8 Family) HD Audio Controller" },
    { pci_vendor_device_key(0x8086, 0x284F),                  "82801H (ICH8 Family) Thermal Reporting Device" },
    { pci_vendor_device_key(0x8086, 0x2918),                        "82801IB (ICH9) LPC Interface Controller" },
    { pci_vendor_device_key(0x8086, 0x2922), "82801IR/IO/IH (ICH9R/DO/DH) 6 port SATA Controller [AHCI mode]" },
    { pci_vendor_device_key(0x8086, 0x2930),                          "82801I (ICH9 Family) SMBus Controller" },
    { pci_vendor_device_key(0x8086, 0x29C0),                      "82G33/G31/P35/P31 Express DRAM Controller" },
    { pci_vendor_device_key(0x8086, 0x2A00),                 "Mobile PM965/GM965/GL960 Memory Controller Hub" },
    { pci_vendor_device_key(0x8086, 0x2A01),                 "Mobile PM965/GM965/GL960 PCI Express Root Port" },
    { pci_vendor_device_key(0x8086, 0x4229),        "PRO/Wireless 4965 AG or AGN [Kedron] Network Connection" },

    { pci_vendor_device_key(0x10DE, 0x060C),                                       "G92M [GeForce 8800M GTX]" },

    { pci_vendor_device_key(0x10EC, 0x8161),      "RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller" },
    { pci_vendor_device_key(0x10EC, 0x8167),                             "RTL-8110SC/8169SC Gigabit Ethernet" },
    { pci_vendor_device_key(0x10EC, 0x8168),      "RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller" },
    { pci_vendor_device_key(0x10EC, 0x8169),                        "RTL8169 PCI Gigabit Ethernet Controller" },
    { pci_vendor_device_key(0x10EC, 0x816A),                                              "RTL8111xP UART #1" },
    { pci_vendor_device_key(0x10EC, 0x816B),                                              "RTL8111xP UART #2" },
    { pci_vendor_device_key(0x10EC, 0x816C),                                       "RTL8111xP IPMI interface" },
    { pci_vendor_device_key(0x10EC, 0x816D),                                   "RTL811x EHCI host controller" },
    { pci_vendor_device_key(0x10EC, 0x816E),                                         "Realtek RealManage BMC" },

    { pci_vendor_device_key(0x1234, 0x1111),                                         "Generic VGA Controller" },
};

cstr kernel::GetPciDeviceName(
    u16 vendor_id,
    u16 device_id)
{
    auto key = pci_vendor_device_key(vendor_id, device_id);
    for (auto& entry : pci_vendor_device_table)
        if (entry.key == key)
            return entry.val;
    return nullptr;
}
