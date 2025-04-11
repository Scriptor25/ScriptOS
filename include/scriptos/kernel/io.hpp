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

void outb(u16 port, u8 value);
void outw(u16 port, u16 value);
void outl(u16 port, u32 value);

u8 inb(u16 port);
u16 inw(u16 port);
u32 inl(u16 port);

void outsb(u16 port, const void *address, u16 count);
void outsw(u16 port, const void *address, u16 count);
void outsl(u16 port, const void *address, u16 count);

void *insb(u16 port, void *address, u16 count);
void *insw(u16 port, void *address, u16 count);
void *insl(u16 port, void *address, u16 count);

/**
 * Write something to the bus to wait one cycle
 */
void io_wait();
