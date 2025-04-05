#pragma once

#include <scriptos/kernel/acpi.hpp>

namespace PCI
{
    struct Command_Register
    {
        bool IOSpace : 1;
        bool MemorySpace : 1;
        bool BusMaster : 1;
        bool SpecialCycles : 1;
        bool MemoryWriteAndInvalidateEnable : 1;
        bool VGAPaletteSnoop : 1;
        bool ParityErrorResponse : 1;
        u8 : 1;
        bool SERREnable : 1;
        bool FastBackToBackEnable : 1;
        bool InterruptDisable : 1;
        u8 : 5;
    } __attribute__((packed));

    struct Status_Register
    {
        u8 : 3;
        bool InterruptStatus : 1;
        bool CapabilitiesList : 1;
        bool Is66MHzCapable : 1;
        u8 : 1;
        bool FastBackToBackCapable : 1;
        bool MasterDataParityError : 1;
        u8 DEVSELTiming : 2;
        bool SignaledTargetAbort : 1;
        bool ReceivedTargetAbort : 1;
        bool ReceivedMasterAbort : 1;
        bool SignaledSystemError : 1;
        bool DetectedParityError : 1;
    } __attribute__((packed));

    struct Type_Register
    {
        u8 Type : 7;
        bool MF : 1;
    } __attribute__((packed));

    struct BIST_Register
    {
        u8 CompletionCode : 4;
        u8 : 2;
        bool StartBIST : 1;
        bool BISTCapable : 1;
    } __attribute__((packed));

    struct BaseAddress_Register
    {
        bool IsIO : 1;
        u32 : 31;
    } __attribute__((packed));

    struct Memory_BaseAddress_Register
    {
        /** Always 0 */
        bool IsIO : 1;
        u8 Type : 2;
        bool Prefetchable : 1;
        /** 16-Byte Aligned Base Address */
        u32 BaseAddress : 28;
    } __attribute__((packed));

    struct IO_BaseAddress_Register
    {
        /** Always 1 */
        bool IsIO : 1;
        u8 : 1;
        /** 4-Byte Aligned Base Address */
        u32 BaseAddress : 30;
    } __attribute__((packed));

    struct Device_Header
    {
        u16 VendorID;
        u16 DeviceID;
        Command_Register Command;
        Status_Register Status;
        u8 RevisionID;
        u8 ProgIF;
        u8 Subclass;
        u8 ClassCode;
        u8 CacheLineSize;
        u8 LatencyTimer;
        Type_Register Type;
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
        u32 CardbusCISPointer;
        u16 SubsystemVendorID;
        u16 SubsystemID;
        u32 ExpansionROMBaseAddress;
        u8 CapabilitiesPointer;
        u8 _1[7];
        u8 InterruptLine;
        u8 InterruptPin;
        u8 MinGrant;
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
        u8 _1[3];
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
        u8 _1;
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

    void EnumeratePCI(ACPI::MCFG_Header *mcfg);
    void EnumerateBus(uptr base_address, uptr bus);
    void EnumerateDevice(uptr bus_address, uptr device);
    void EnumerateFunction(uptr device_address, uptr function);
}
