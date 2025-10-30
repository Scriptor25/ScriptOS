#pragma once

#include <scriptos/types.h>

#define INT(I)                 asm volatile("int $" #I)
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

inline void hlt()
{
    asm volatile("hlt");
}

inline void cli()
{
    asm volatile("cli");
}

inline void sti()
{
    asm volatile("sti");
}

inline void wrmsr(
    u32 msr,
    u64 value)
{
    auto lo = static_cast<u32>(value & 0xFFFFFFFFu);
    auto hi = static_cast<u32>((value >> 32) & 0xFFFFFFFFu);

    asm volatile("wrmsr" : : "c"(msr), "a"(lo), "d"(hi) : "memory");
}

inline u64 rdmsr(u32 msr)
{
    u32 lo, hi;
    asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
    return static_cast<u64>(hi) << 32 | lo;
}
