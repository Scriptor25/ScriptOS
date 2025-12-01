#include <scriptos/acpi.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>

const acpi::SystemDescriptorTableHeader* acpi::SystemDescriptorTable::Find(cstr signature) const
{
    for (auto header : *this)
    {
        kernel::MapPage(header, header);

        if (kernel::Compare(header->Signature, signature, 4))
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

const acpi::SystemDescriptorTableHeader* acpi::ExtendedSystemDescriptorTable::Find(cstr signature) const
{
    for (auto header : *this)
    {
        kernel::MapPage(header, header);

        if (kernel::Compare(header->Signature, signature, 4))
        {
            return header;
        }
    }

    return nullptr;
}

acpi::TableIterator<acpi::ExtendedSystemDescriptorTable> acpi::ExtendedSystemDescriptorTable::begin() const
{
    return { *this, 0 };
}

acpi::TableIterator<acpi::ExtendedSystemDescriptorTable> acpi::ExtendedSystemDescriptorTable::end() const
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
