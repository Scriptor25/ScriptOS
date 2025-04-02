#include <scriptos/kernel/acpi.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>

bool RSDP::Validate()
{
    u8 sum = 0;

    for (usize i = 0; i < sizeof(RSDP); ++i)
        sum += *(reinterpret_cast<u8 *>(this) + i);

    return sum == 0;
}

bool XSDP::Validate()
{
    u8 sum = 0;

    for (usize i = 0; i < sizeof(XSDP); ++i)
        sum += *(reinterpret_cast<u8 *>(this) + i);

    return sum == 0;
}

bool SDT_Header::Validate()
{
    u8 sum = 0;

    for (usize i = 0; i < Length; ++i)
        sum += *(reinterpret_cast<u8 *>(this) + i);

    return sum == 0;
}

SDT_Header *RSDT::Find(cstr signature)
{
    usize num_entries = (Header.Length - sizeof(SDT_Header)) / 4;

    for (usize i = 0; i < num_entries; ++i)
    {
        auto header = reinterpret_cast<SDT_Header *>(PointerToOtherSDT[i]);
        if (!memcmp(header->Signature, signature, 4))
            return header;
    }

    return nullptr;
}
