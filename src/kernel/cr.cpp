#include <scriptos/kernel/cr.hpp>

CR::CR0 CR::CR0::R()
{
    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    return cr0;
}

void CR::CR0::W(CR0 cr0)
{
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

CR::CR0::CR0(u32 value)
    : Value(value)
{
}

CR::CR0::operator u32() const
{
    return Value;
}

CR::CR2 CR::CR2::R()
{
    u32 cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2));
    return cr2;
}

void CR::CR2::W(CR2 cr2)
{
    asm volatile("mov %0, %%cr2" : : "r"(cr2));
}

CR::CR2::CR2(u32 value)
    : Value(value)
{
}

CR::CR2::operator u32() const
{
    return Value;
}

CR::CR3 CR::CR3::R()
{
    u32 cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

void CR::CR3::W(CR3 cr3)
{
    asm volatile("mov %0, %%cr3" : : "r"(cr3));
}

CR::CR3::CR3(u32 value)
    : Value(value)
{
}

CR::CR3::operator u32() const
{
    return Value;
}

CR::CR4 CR::CR4::R()
{
    u32 cr4;
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    return cr4;
}

void CR::CR4::W(CR4 cr4)
{
    asm volatile("mov %0, %%cr4" : : "r"(cr4));
}

CR::CR4::CR4(u32 value)
    : Value(value)
{
}

CR::CR4::operator u32() const
{
    return Value;
}

CR::CR8 CR::CR8::R()
{
    u32 cr8;
    asm volatile("mov %%cr8, %0" : "=r"(cr8));
    return cr8;
}

void CR::CR8::W(CR8 cr8)
{
    asm volatile("mov %0, %%cr8" : : "r"(cr8));
}

CR::CR8::CR8(u32 value)
    : Value(value)
{
}

CR::CR8::operator u32() const
{
    return Value;
}
