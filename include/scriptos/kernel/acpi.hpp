#pragma once

#include <scriptos/std/types.hpp>

namespace ACPI
{
    struct RSDP
    {
        bool Validate() const;

        char Signature[8];
        u8 Checksum;
        char OEMID[6];
        u8 Revision;
        u32 RSDTAddress;
    } __attribute__((packed));

    struct XSDP
    {
        bool Validate() const;

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
        bool Validate() const;

        char Signature[4];
        u32 Length;
        u8 Revision;
        u8 Checksum;
        char OEMID[6];
        char OEMTableID[8];
        u32 OEMRevision;
        u32 CreatorID;
        u32 CreatorRevision;
    } __attribute__((packed));

    struct RSDT
    {
        SDT_Header* Find(cstr signature) const;

        template<typename T>
        void ForEach(T consumer) const
        {
            auto num_entries = (Header.Length - sizeof(SDT_Header)) / 4;

            for (usize i = 0; i < num_entries; ++i)
            {
                auto header = Entries[i];
                if (consumer(header))
                    break;
            }
        }

        SDT_Header Header;
        SDT_Header* Entries[0];
    } __attribute__((packed));

    struct MCFG_Entry
    {
        u32 BaseAddressLo;
        u32 BaseAddressHi;
        u16 PCISegmentGroup;
        u8 StartPCIBus;
        u8 EndPCIBus;
        u8 _1[4];
    } __attribute__((packed));

    struct MCFG_Header
    {
        template<typename T>
        void ForEach(T consumer) const
        {
            auto num_entries = (Header.Length - sizeof(MCFG_Header)) / sizeof(MCFG_Entry);

            for (usize i = 0; i < num_entries; ++i)
                consumer(Entries[i]);
        }

        SDT_Header Header;
        u8 _1[8];

        MCFG_Entry Entries[0];
    } __attribute__((packed));
}
