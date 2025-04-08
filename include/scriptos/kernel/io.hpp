#pragma once

#include <scriptos/std/types.hpp>

/**
 * Throw an interrupt
 */
#define INT(N) asm volatile("int $" #N)

/**
 * Push X to the stack
 */
#define PUSH(X) asm volatile("push %0" : : "r"(X))
/**
 * Pop from the stack to X
 */
#define POP(X) asm volatile("pop %0" : "=r"(X))

/**
 * Infinite loop
 */
#define LOOP() asm volatile("jmp .")
/**
 * Halt CPU execution
 */
#define HLT() asm volatile("hlt")

/**
 * Disable maskable interrupts
 */
#define CLI() asm volatile("cli")
/**
 * Enable maskable interrupts
 */
#define STI() asm volatile("sti")

/**
 * Write a value to a port
 */
template <typename T>
void out(u16 port, T value)
{
    asm volatile("out %0, %1" : : "r"(value), "Nd"(port));
}

/**
 * Read a value from a port
 */
template <typename T>
T in(u16 port)
{
    T value;
    asm volatile("in %1, %0" : "=r"(value) : "Nd"(port));
    return value;
}

void outb(u16 port, u8 value);
void outw(u16 port, u16 value);
void outl(u16 port, u32 value);

u8 inb(u16 port);
u16 inw(u16 port);
u32 inl(u16 port);

/**
 * Write something to the bus to wait one cycle
 */
void io_wait();
