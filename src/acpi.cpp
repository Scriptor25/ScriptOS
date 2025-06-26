#include <scriptos/acpi.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>

const acpi::SDT_Header* acpi::RSDT::Find(cstr signature) const
{
    for (auto header : *this)
    {
        paging::MapPage(header, header);

        if (memory::Compare(header->Signature, signature, 4))
            return header;
    }

    return nullptr;
}

acpi::Iterator<acpi::RSDT> acpi::RSDT::begin() const
{
    return { *this, 0 };
}

acpi::Iterator<acpi::RSDT> acpi::RSDT::end() const
{
    auto count = (Header.Length - sizeof(SDT_Header)) / sizeof(u32);
    return { *this, count };
}

const acpi::SDT_Header* acpi::XSDT::Find(cstr signature) const
{
    for (auto header : *this)
    {
        paging::MapPage(header, header);

        if (memory::Compare(header->Signature, signature, 4))
            return header;
    }

    return nullptr;
}

acpi::Iterator<acpi::XSDT> acpi::XSDT::begin() const
{
    return { *this, 0 };
}

acpi::Iterator<acpi::XSDT> acpi::XSDT::end() const
{
    auto count = (Header.Length - sizeof(SDT_Header)) / sizeof(u64);
    return { *this, count };
}

const acpi::MCFG_Entry* acpi::MCFG::begin() const
{
    return Table;
}

const acpi::MCFG_Entry* acpi::MCFG::end() const
{
    return Table + ((Header.Length - sizeof(SDT_Header)) / sizeof(MCFG_Entry));
}
