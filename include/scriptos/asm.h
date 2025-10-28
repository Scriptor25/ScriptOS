#pragma once

#include <scriptos/types.h>

#define INT(I) asm volatile("int $" #I)

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
