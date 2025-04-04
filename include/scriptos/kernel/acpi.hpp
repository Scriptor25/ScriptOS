#pragma once

#include <scriptos/std/types.hpp>

namespace ACPI
{
    struct RSDP
    {
        /**
         * Validate this RSDP
         */
        bool Validate();

        char Signature[8];
        u8 Checksum;
        char OEMID[6];
        u8 Revision;
        u32 RSDTAddress;
    } __attribute__((packed));

    struct XSDP
    {
        /**
         * Validate this XSDP
         */
        bool Validate();

        char Signature[8];
        u8 Checksum;
        char OEMID[6];
        u8 Revision;
        u32 RSDTAddress;

        u32 Length;
        u32 XSDTAddressLo;
        u32 XSDTAddressHi;
        u8 ExtendedChecksum;

        u8 _1[3];
    } __attribute__((packed));

    struct SDT_Header
    {
        /**
         * Validate this SDT header
         */
        bool Validate();

        char Signature[4];
        u32 Length;
        u8 Revision;
        u8 Checksum;
        char OEMID[6];
        char OEMTableID[8];
        u32 OEMRevision;
        u32 CreatorID;
        u32 CreatorRevision;
    };

    /**
     * RSD table
     */
    struct RSDT
    {
        /**
         * Find a SDT header in this RSD table
         */
        SDT_Header *Find(cstr signature);

        SDT_Header Header;
        u32 *PointerToOtherSDT;
    };

    struct GenericAddressStructure
    {
        u8 AddressSpace;
        u8 BitWidth;
        u8 BitOffset;
        u8 AccessSize;
        u32 Address_Lo;
        u32 Address_Hi;
    };

    /**
     * FAD table
     */
    struct FADT
    {
        SDT_Header Header;

        u32 FirmwareCtrl;
        u32 Dsdt;

        u8 _1;

        u8 PreferredPowerManagementProfile;
        u16 SCI_Interrupt;
        u32 SMI_CommandPort;
        u8 AcpiEnable;
        u8 AcpiDisable;
        u8 S4BIOS_REQ;
        u8 PSTATE_Control;
        u32 PM1aEventBlock;
        u32 PM1bEventBlock;
        u32 PM1aControlBlock;
        u32 PM1bControlBlock;
        u32 PM2ControlBlock;
        u32 PMTimerBlock;
        u32 GPE0Block;
        u32 GPE1Block;
        u8 PM1EventLength;
        u8 PM1ControlLength;
        u8 PM2ControlLength;
        u8 PMTimerLength;
        u8 GPE0Length;
        u8 GPE1Length;
        u8 GPE1Base;
        u8 CStateControl;
        u16 WorstC2Latency;
        u16 WorstC3Latency;
        u16 FlushSize;
        u16 FlushStride;
        u8 DutyOffset;
        u8 DutyWidth;
        u8 DayAlarm;
        u8 MonthAlarm;
        u8 Century;

        u16 BootArchitectureFlags;

        u8 _2;
        u32 Flags;

        GenericAddressStructure ResetReg;

        u8 ResetValue;
        u8 _3[3];

        u64 X_FirmwareControl;
        u64 X_Dsdt;

        GenericAddressStructure X_PM1aEventBlock;
        GenericAddressStructure X_PM1bEventBlock;
        GenericAddressStructure X_PM1aControlBlock;
        GenericAddressStructure X_PM1bControlBlock;
        GenericAddressStructure X_PM2ControlBlock;
        GenericAddressStructure X_PMTimerBlock;
        GenericAddressStructure X_GPE0Block;
        GenericAddressStructure X_GPE1Block;
    };

    /**
     * DSD table
     */
    struct DSDT
    {
        SDT_Header Header;
    };
}
