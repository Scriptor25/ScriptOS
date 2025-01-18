#pragma once

#include <scriptos/std/types.hpp>

#define INT(N) asm volatile("int $" #N)

#define PUSH(X) asm volatile("push %0" : : "r"(X))
#define POP(X) asm volatile("pop %0" : "=r"(X))

#define LOOP() asm volatile("jmp .")
#define HLT() asm volatile("hlt")

#define CLI() asm volatile("cli")
#define STI() asm volatile("sti")

template <typename T>
void out(u16 port, T value)
{
    asm volatile("out %0, %1" : : "r"(value), "Nd"(port));
}

template <typename T>
T in(u16 port)
{
    T value;
    asm volatile("in %1, %0" : "=r"(value) : "Nd"(port));
    return value;
}

void io_wait();

void lidt(const struct IDT_Descriptor &idt);
void invlpg(void *address);
