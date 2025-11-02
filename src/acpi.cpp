#include <scriptos/acpi.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>

const acpi::SdtHeader* acpi::RsdTable::Find(cstr signature) const
{
    for (auto header : *this)
    {
        paging::MapPage(header, header);

        if (memory::Compare(header->Signature, signature, 4))
        {
            return header;
        }
    }

    return nullptr;
}

acpi::TableIterator<acpi::RsdTable> acpi::RsdTable::begin() const
{
    return { *this, 0 };
}

acpi::TableIterator<acpi::RsdTable> acpi::RsdTable::end() const
{
    auto count = (Header.Length - sizeof(SdtHeader)) / sizeof(u32);
    return { *this, count };
}

const acpi::SdtHeader* acpi::XsdTable::Find(cstr signature) const
{
    for (auto header : *this)
    {
        paging::MapPage(header, header);

        if (memory::Compare(header->Signature, signature, 4))
        {
            return header;
        }
    }

    return nullptr;
}

acpi::TableIterator<acpi::XsdTable> acpi::XsdTable::begin() const
{
    return { *this, 0 };
}

acpi::TableIterator<acpi::XsdTable> acpi::XsdTable::end() const
{
    auto count = (Header.Length - sizeof(SdtHeader)) / sizeof(u64);
    return { *this, count };
}

const acpi::McfgEntry* acpi::Mcfg::begin() const
{
    return Table;
}

const acpi::McfgEntry* acpi::Mcfg::end() const
{
    return Table + ((Header.Length - sizeof(SdtHeader)) / sizeof(McfgEntry));
}
