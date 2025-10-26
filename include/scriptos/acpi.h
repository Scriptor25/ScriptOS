#pragma once

#include <scriptos/paging.h>
#include <scriptos/types.h>

namespace acpi
{
    struct RSDP
    {
        char Signature[8];
        u8 Checksum;
        char OEMID[6];
        u8 Revision;
        u32 RSDT_Address;
    } __attribute__((packed));

    struct XSDP
    {
        char Signature[8];
        u8 Checksum;
        char OEMID[6];
        u8 Revision;
        u32 RSDT_Address;

        u32 Length;
        u64 XSDT_Address;
        u8 ExtendedChecksum;
        u8 Reserved[3];
    } __attribute__((packed));

    struct SDT_Header
    {
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

    template<typename BaseT>
    struct Iterator
    {
        bool operator==(const Iterator& iterator) const
        {
            return &Base == &iterator.Base && Offset == iterator.Offset;
        }

        const SDT_Header* operator*() const
        {
            return reinterpret_cast<SDT_Header*>(Base.Table[Offset]);
        }

        Iterator& operator++()
        {
            Offset++;
            return *this;
        }

        const BaseT& Base;
        usize Offset;
    };

    struct RSDT
    {
        template<typename T>
        const T* Find(cstr signature) const
        {
            return reinterpret_cast<const T*>(Find(signature));
        }

        const SDT_Header* Find(cstr signature) const;

        Iterator<RSDT> begin() const;
        Iterator<RSDT> end() const;

        SDT_Header Header;
        u32 Table[];
    } __attribute__((packed));

    struct XSDT
    {
        template<typename T>
        const T* Find(cstr signature) const
        {
            return reinterpret_cast<const T*>(Find(signature));
        }

        const SDT_Header* Find(cstr signature) const;

        Iterator<XSDT> begin() const;
        Iterator<XSDT> end() const;

        SDT_Header Header;
        u64 Table[];
    } __attribute__((packed));

    struct MCFG_Entry
    {
        u64 BaseAddress;
        u16 SegmentGroup;
        u8 StartBus;
        u8 EndBus;
        u8 Reserved[4];
    } __attribute__((packed));

    struct MCFG
    {
        const MCFG_Entry* begin() const;
        const MCFG_Entry* end() const;

        SDT_Header Header;
        u8 Reserved[8];

        MCFG_Entry Table[];
    } __attribute__((packed));
}
