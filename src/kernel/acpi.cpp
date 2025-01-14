#include <scriptos/kernel/acpi.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>

bool RSDP::Validate()
{
    u8 sum = 0;

    for (usize i = 0; i < sizeof(RSDP); ++i)
        sum += *((u8 *)this + i);

    return sum == 0;
}

bool XSDP::Validate()
{
    u8 sum = 0;

    for (usize i = 0; i < sizeof(XSDP); ++i)
        sum += *((u8 *)this + i);

    return sum == 0;
}

bool SDT_Header::Validate()
{
    u8 sum = 0;

    for (usize i = 0; i < Length; ++i)
        sum += *((u8 *)this + i);

    return sum == 0;
}

SDT_Header *RSDT::Find(cstr signature)
{
    usize num_entries = (Header.Length - sizeof(SDT_Header)) / 4;

    for (usize i = 0; i < num_entries; ++i)
    {
        auto header = (SDT_Header *)PointerToOtherSDT[i];
        if (!memcmp(header->Signature, signature, 4))
            return header;
    }

    return nullptr;
}

void ACPI_Init(RSDP *rsdp)
{
    if (!rsdp)
    {
        printf("rsdp must not be null");
        return;
    }

    if (!rsdp->Validate())
    {
        printf("invalid rsdp");
        return;
    }

    auto rsdt = (RSDT *)rsdp->RSDTAddress;
    if (auto header = rsdt->Find("RSDT"); !header || !header->Validate())
    {
        printf("no rsdt header");
        return;
    }

    auto facp = (FADT *)rsdt->Find("FACP");
    facp->PM1aControlBlock;
    facp->PM1bControlBlock;

    auto dsdt = (DSDT *)facp->Dsdt;
}

void ACPI_Enable()
{
}
