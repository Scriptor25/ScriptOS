#include <scriptos/kernel/io.hpp>

void io_wait()
{
    out<u8>(0x80, 0);
}

void lidt(const struct IDT_Descriptor &idt)
{
    asm volatile("lidt (%0)" : : "r"(&idt));
}

void invlpg(void *address)
{
    asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}
