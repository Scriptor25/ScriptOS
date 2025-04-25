#pragma once

#include <scriptos/kernel/acpi.hpp>

namespace PCI
{
    struct Command_Register
    {
        /** If set to 1 the device can respond to I/O Space accesses; otherwise, the device's response is disabled. */
        bool IOSpace : 1;
        /**
         * If set to 1 the device can respond to Memory Space accesses; otherwise, the device's response is disabled.
         */
        bool MemorySpace : 1;
        /** If set to 1 the device can behave as a bus master; otherwise, the device can not generate PCI accesses. */
        bool BusMaster : 1;
        /** If set to 1 the device can monitor Special Cycle operations; otherwise, the device will ignore them. */
        bool SpecialCycles : 1;
        /**
         * If set to 1 the device can generate the Memory Write and Invalidate command; otherwise, the Memory Write
         * command must be used.
         */
        bool MemoryWriteAndInvalidateEnable : 1;
        /**
         * If set to 1 the device does not respond to palette register writes and will snoop the data; otherwise, the
         * device will trate palette write accesses like all other accesses.
         */
        bool VGAPaletteSnoop : 1;
        /**
         * If set to 1 the device will take its normal action when a parity error is detected; otherwise, when an error
         * is detected, the device will set bit 15 of the Status register (Detected Parity Error Status Bit), but will
         * not assert the PERR# (Parity Error) pin and will continue operation as normal.
         */
        bool ParityErrorResponse : 1;
        /**
         * Reserved. As of revision 3.0 of the PCI local bus specification this bit is hardwired to 0. In earlier
         * versions of the specification this bit was used by devices and may have been hardwired to 0, 1, or
         * implemented as a read/write bit.
         */
        u8 _0 : 1;
        /** If set to 1 the SERR# driver is enabled; otherwise, the driver is disabled. */
        bool SERREnable : 1;
        /**
         * If set to 1 indicates a device is allowed to generate fast back-to-back transactions; otherwise, fast
         * back-to-back transactions are only allowed to the same agent.
         */
        bool FastBackToBackEnable : 1;
        /**
         * If set to 1 the assertion of the devices INTx# signal is disabled; otherwise, assertion of the signal is
         * enabled.
         */
        bool InterruptDisable : 1;
        /** Reserved */
        u8 _1 : 5;
    } __attribute__((packed));

    struct Status_Register
    {
        /** Reserved */
        u8 _0 : 3;
        /**
         * Represents the state of the device's INTx# signal. If set to 1 and bit 10 of the Command register (Interrupt
         * Disable bit) is set to 0 the signal will be asserted; otherwise, the signal will be ignored.
         */
        bool InterruptStatus : 1;
        /**
         * If set to 1 the device implements the pointer for a New Capabilities Linked list at offset 0x34; otherwise,
         * the linked list is not available.
         */
        bool CapabilitiesList : 1;
        /** If set to 1 the device is capable of running at 66 MHz; otherwise, the device runs at 33 MHz. */
        bool Is66MHzCapable : 1;
        /**
         * Reserved. As of revision 3.0 of the PCI Local Bus specification this bit is reserved. In revision 2.1 of the
         * specification this bit was used to indicate whether or not a device supported User Definable Features.
         */
        u8 _1 : 1;
        /**
         * If set to 1 the device can accept fast back-to-back transactions that are not from the same agent;
         * otherwise, transactions can only be accepted from the same agent.
         */
        bool FastBackToBackCapable : 1;
        /**
         * This bit is only set when the following conditions are met. The bus agent asserted PERR# on a read or
         * observed an assertion of PERR# on a write, the agent setting the bit acted as the bus master for the
         * operation in which the error occurred, and bit 6 of the Command register (Parity Error Response bit) is set
         * to 1.
         */
        bool MasterDataParityError : 1;
        /**
         * Read only bits that represent the slowest time that a device will assert DEVSEL# for any bus command except
         * Configuration Space read and writes. Where a value of 0x0 represents fast timing, a value of 0x1 represents
         * medium timing, and a value of 0x2 represents slow timing.
         */
        u8 DEVSELTiming : 2;
        /** This bit will be set to 1 whenever a target device terminates a transaction with Target-Abort. */
        bool SignaledTargetAbort : 1;
        /** This bit will be set to 1, by a master device, whenever its transaction is terminated with Target-Abort. */
        bool ReceivedTargetAbort : 1;
        /**
         * This bit will be set to 1, by a master device, whenever its transaction (except for Special Cycle
         * transactions) is terminated with Master-Abort.
         */
        bool ReceivedMasterAbort : 1;
        /** This bit will be set to 1 whenever the device asserts SERR#. */
        bool SignaledSystemError : 1;
        /**
         * This bit will be set to 1 whenever the device detects a parity error, even if parity error handling is
         * disabled.
         */
        bool DetectedParityError : 1;
    } __attribute__((packed));

    struct Type_Register
    {
        /** 0x0 Standard Header - 0x1 PCI-to-PCI Bridge - 0x2 CardBus Bridge */
        u8 Type : 7;
        /** If MF = 1 Then this device has multiple functions. */
        bool MF : 1;
    } __attribute__((packed));

    struct BIST_Register
    {
        /** Will return 0, after BIST execution, if the test completed successfully. */
        u8 CompletionCode : 4;
        /** Reserved */
        u8 _0 : 2;
        /**
         * When set to 1 the BIST is invoked. This bit is reset when BIST completes. If BIST does not complete after 2
         * seconds the device should be failed by system software.
         */
        bool StartBIST : 1;
        /** Will return 1 the device supports BIST. */
        bool BISTCapable : 1;
    } __attribute__((packed));

    struct BaseAddress_Register
    {
        bool IsIO : 1;
        u32 _0    : 31;
    } __attribute__((packed));

    struct Memory_BaseAddress_Register
    {
        /** false */
        bool IsIO         : 1;
        u8 Type           : 2;
        bool Prefetchable : 1;
        /** 16-Byte Aligned Base Address */
        u32 BaseAddress : 28;
    } __attribute__((packed));

    struct IO_BaseAddress_Register
    {
        /** true */
        bool IsIO : 1;
        /** Reserved */
        u8 _0 : 1;
        /** 4-Byte Aligned Base Address */
        u32 BaseAddress : 30;
    } __attribute__((packed));

    struct Device_Header
    {
        /**
         * Identifies the manufacturer of the device. Where valid IDs are allocated by PCI-SIG to ensure uniqueness and
         * 0xFFFF is an invalid value that will be returned on read accesses to Configuration Space registers of
         * non-existent devices.
         */
        u16 VendorID;
        /** Identifies the particular device. Where valid IDs are allocated by the vendor. */
        u16 DeviceID;
        /**
         * Provides control over a device's ability to generate and respond to PCI cycles. Where the only functionality
         * guaranteed to be supported by all devices is, when a 0 is written to this register, the device is
         * disconnected from the PCI bus for all accesses except Configuration Space access.
         */
        Command_Register Command;
        /** A register used to record status information for PCI bus related events. */
        Status_Register Status;
        /** Specifies a revision identifier for a particular device. Where valid IDs are allocated by the vendor. */
        u8 RevisionID;
        /**
         * A read-only register that specifies a register-level programming interface the device has, if it has any at
         * all.
         */
        u8 ProgIF;
        /** A read-only register that specifies the specific function the device performs. */
        u8 Subclass;
        /** A read-only register that specifies the type of function the device performs. */
        u8 ClassCode;
        /**
         * Specifies the system cache line size in 32-bit units. A device can limit the number of cacheline sizes it
         * can support, if a unsupported value is written to this field, the device will behave as if a value of 0 was
         * written.
         */
        u8 CacheLineSize;
        /** Specifies the latency timer in units of PCI bus clocks. */
        u8 LatencyTimer;
        /**
         * Identifies the layout of the rest of the header beginning at byte 0x10 of the header. If bit 7 of this
         * register is set, the device has multiple functions; otherwise, it is a single function device. Types:
         *   - 0x0: a general device
         *   - 0x1: a PCI-to-PCI bridge
         *   - 0x2: a PCI-to-CardBus bridge.
         */
        Type_Register HeaderType;
        /** Represents that status and allows control of a devices BIST (built-in self test). */
        BIST_Register BIST;
    } __attribute__((packed));

    struct Device_Type_0
    {
        Device_Header Header;
        BaseAddress_Register BaseAddress0;
        BaseAddress_Register BaseAddress1;
        BaseAddress_Register BaseAddress2;
        BaseAddress_Register BaseAddress3;
        BaseAddress_Register BaseAddress4;
        BaseAddress_Register BaseAddress5;
        /**
         * Points to the Card Information Structure and is used by devices that share silicon between CardBus and PCI.
         */
        u32 CardbusCISPointer;
        u16 SubsystemVendorID;
        u16 SubsystemID;
        u32 ExpansionROMBaseAddress;
        /**
         * Points (i.e. an offset into this function's configuration space) to a linked list of new capabilities
         * implemented by the device. Used if bit 4 of the status register (Capabilities List bit) is set to 1. The
         * bottom two bits are reserved and should be masked before the Pointer is used to access the Configuration
         * Space.
         */
        u8 CapabilitiesPointer;
        /** Reserved */
        u8 _0[7];
        /**
         * Specifies which input of the system interrupt controllers the device's interrupt pin is connected to and is
         * implemented by any device that makes use of an interrupt pin. For the x86 architecture this register
         * corresponds to the PIC IRQ numbers 0-15 (and not I/O APIC IRQ numbers) and a value of 0xFF defines no
         * connection.
         */
        u8 InterruptLine;
        /**
         * Specifies which interrupt pin the device uses. Where a value of 0x1 is INTA#, 0x2 is INTB#, 0x3 is INTC#,
         * 0x4 is INTD#, and 0x0 means the device does not use an interrupt pin.
         */
        u8 InterruptPin;
        /**
         * A read-only register that specifies the burst period length, in 1/4 microsecond units, that the device needs
         * (assuming a 33 MHz clock rate).
         */
        u8 MinGrant;
        /**
         * A read-only register that specifies how often the device needs access to the PCI bus (in 1/4 microsecond
         * units).
         */
        u8 MaxLatency;
    } __attribute__((packed));

    struct Device_Type_1
    {
        Device_Header Header;
        BaseAddress_Register BaseAddress0;
        BaseAddress_Register BaseAddress1;
        u8 PrimaryBusNumber;
        u8 SecondaryBusNumber;
        u8 SubordinateBusNumber;
        u8 SecondaryLatencyTimer;
        u8 IOBase;
        u8 IOLimit;
        u16 SecondaryStatus;
        u16 MemoryBase;
        u16 MemoryLimit;
        u16 PrefetchableMemoryBase;
        u16 PrefetchableMemoryLimit;
        u32 PrefetchableBaseUpper32Bits;
        u32 PrefetchableLimitUpper32Bits;
        u16 IOBaseUpper16Bits;
        u16 IOLimitUpper16Bits;
        u8 CapabilityPointer;
        u8 _0[3];
        u32 ExpansionROMBaseAddress;
        u8 InterruptLine;
        u8 InterruptPin;
        u16 BridgeControl;
    } __attribute__((packed));

    struct Device_Type_2
    {
        Device_Header Header;
        u32 CardBusSocketExCaBaseAddress;
        u8 OffsetOfCapabilitiesList;
        u8 _0;
        u16 SecondaryStatus;
        u8 PCIBus;
        u8 CardBus;
        u8 SubordinateBus;
        u8 CardBusLatencyTimer;
        u32 MemoryBaseAddress0;
        u32 MemoryLimit0;
        u32 MemoryBaseAddress1;
        u32 MemoryLimit1;
        u32 IOBaseAddress0;
        u32 IOLimit0;
        u32 IOBaseAddress1;
        u32 IOLimit1;
        u8 InterruptLine;
        u8 InterruptPin;
        u16 BridgeControl;
        u16 SubsystemDeviceID;
        u16 SubsystemVendorID;
        u32 PC16BitCardLegacyModeBaseAddress;
    } __attribute__((packed));

    cstr GetDeviceDescriptor(u8 class_code = 0xFF, u8 subclass = 0xFF, u8 prog_if = 0xFF);
    cstr GetVendorName(u16 vendor_id);
    cstr GetDeviceName(u16 vendor_id, u16 device_id);

    void EnumeratePCI(ACPI::MCFG_Header* mcfg);
    void EnumerateBus(uptr base_address, uptr bus);
    void EnumerateDevice(uptr bus_address, uptr device);
    void EnumerateFunction(uptr device_address, uptr function);
}
