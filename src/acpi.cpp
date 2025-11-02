#include <scriptos/acpi.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>

const acpi::SystemDescriptorTableHeader* acpi::SystemDescriptorTable::Find(cstr signature) const
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

acpi::TableIterator<acpi::SystemDescriptorTable> acpi::SystemDescriptorTable::begin() const
{
    return { *this, 0 };
}

acpi::TableIterator<acpi::SystemDescriptorTable> acpi::SystemDescriptorTable::end() const
{
    auto count = (Header.Length - sizeof(SystemDescriptorTableHeader)) / sizeof(u32);
    return { *this, count };
}

const acpi::SystemDescriptorTableHeader* acpi::XSystemDescriptorTable::Find(cstr signature) const
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

acpi::TableIterator<acpi::XSystemDescriptorTable> acpi::XSystemDescriptorTable::begin() const
{
    return { *this, 0 };
}

acpi::TableIterator<acpi::XSystemDescriptorTable> acpi::XSystemDescriptorTable::end() const
{
    auto count = (Header.Length - sizeof(SystemDescriptorTableHeader)) / sizeof(u64);
    return { *this, count };
}

const acpi::McfgEntry* acpi::Mcfg::begin() const
{
    return Table;
}

const acpi::McfgEntry* acpi::Mcfg::end() const
{
    return Table + ((Header.Length - sizeof(SystemDescriptorTableHeader)) / sizeof(McfgEntry));
}
