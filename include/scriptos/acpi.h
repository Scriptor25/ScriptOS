#pragma once

#include <scriptos/paging.h>
#include <scriptos/types.h>

namespace acpi
{
    struct SystemDescriptorPointer
    {
        char Signature[8];
        u8 Checksum;
        char OemId[6];
        u8 Revision;
        u32 RsdtAddress;
    } __attribute__((packed));

    struct XSystemDescriptorPointer
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
    struct SystemDescriptorTableHeader
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

        const SystemDescriptorTableHeader* operator*() const
        {
            return reinterpret_cast<SystemDescriptorTableHeader*>(Base.Table[Offset]);
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
     * System Description Table
     */
    struct SystemDescriptorTable
    {
        template<typename T>
        const T* Find(cstr signature) const
        {
            return reinterpret_cast<const T*>(Find(signature));
        }

        const SystemDescriptorTableHeader* Find(cstr signature) const;

        TableIterator<SystemDescriptorTable> begin() const;
        TableIterator<SystemDescriptorTable> end() const;

        SystemDescriptorTableHeader Header;
        u32 Table[];
    } __attribute__((packed));

    /**
     * Extended System Description Table
     */
    struct XSystemDescriptorTable
    {
        template<typename T>
        const T* Find(cstr signature) const
        {
            return reinterpret_cast<const T*>(Find(signature));
        }

        const SystemDescriptorTableHeader* Find(cstr signature) const;

        TableIterator<XSystemDescriptorTable> begin() const;
        TableIterator<XSystemDescriptorTable> end() const;

        SystemDescriptorTableHeader Header;
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

        SystemDescriptorTableHeader Header;
        u8 _rsv0[8];

        McfgEntry Table[];
    } __attribute__((packed));
}
