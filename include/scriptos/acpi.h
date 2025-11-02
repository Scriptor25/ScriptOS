#pragma once

#include <scriptos/paging.h>
#include <scriptos/types.h>

namespace acpi
{
    /**
     * Root System Description Pointer
     */
    struct RsdPointer
    {
        char Signature[8];
        u8 Checksum;
        char OemId[6];
        u8 Revision;
        u32 RsdtAddress;
    } __attribute__((packed));

    /**
     * Extended Root System Description Pointer
     */
    struct XsdPointer
    {
        char Signature[8];
        u8 Checksum;
        char OemId[6];
        u8 Revision;
        u32 RsdtAddress;

        u32 Length;
        u64 XsdtAddress;
        u8 ExtendedChecksum;
        u8 _rsv0[3];
    } __attribute__((packed));

    /**
     * System Description Table Header
     */
    struct SdtHeader
    {
        char Signature[4];
        u32 Length;
        u8 Revision;
        u8 Checksum;
        char OemId[6];
        char OemTableId[8];
        u32 OemRevision;
        u32 CreatorId;
        u32 CreatorRevision;
    } __attribute__((packed));

    template<typename T>
    struct TableIterator
    {
        bool operator==(const TableIterator& iterator) const
        {
            return &Base == &iterator.Base && Offset == iterator.Offset;
        }

        const SdtHeader* operator*() const
        {
            return reinterpret_cast<SdtHeader*>(Base.Table[Offset]);
        }

        TableIterator& operator++()
        {
            Offset++;
            return *this;
        }

        const T& Base;
        usize Offset;
    };

    /**
     * Root System Description Table
     */
    struct RsdTable
    {
        template<typename T>
        const T* Find(cstr signature) const
        {
            return reinterpret_cast<const T*>(Find(signature));
        }

        const SdtHeader* Find(cstr signature) const;

        TableIterator<RsdTable> begin() const;
        TableIterator<RsdTable> end() const;

        SdtHeader Header;
        u32 Table[];
    } __attribute__((packed));

    /**
     * Extended Root System Description Table
     */
    struct XsdTable
    {
        template<typename T>
        const T* Find(cstr signature) const
        {
            return reinterpret_cast<const T*>(Find(signature));
        }

        const SdtHeader* Find(cstr signature) const;

        TableIterator<XsdTable> begin() const;
        TableIterator<XsdTable> end() const;

        SdtHeader Header;
        u64 Table[];
    } __attribute__((packed));

    struct McfgEntry
    {
        u64 BaseAddress;
        u16 SegmentGroup;
        u8 StartBus;
        u8 EndBus;
        u8 _rsv0[4];
    } __attribute__((packed));

    struct Mcfg
    {
        const McfgEntry* begin() const;
        const McfgEntry* end() const;

        SdtHeader Header;
        u8 _rsv0[8];

        McfgEntry Table[];
    } __attribute__((packed));
}
