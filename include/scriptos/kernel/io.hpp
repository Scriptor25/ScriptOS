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
 * Disable interrupts
 */
#define CLI() asm volatile("cli")
/**
 * Enable interrupts
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

/**
 * Write something to the bus to wait one cycle
 */
void io_wait();

/**
 * Load an IDT from a descriptor
 */
void lidt(const struct IDT_Descriptor &idt);
/**
 * Invalidate a single page. Use this instead of updating the whole IDT to save some performance
 */
void invlpg(void *address);
