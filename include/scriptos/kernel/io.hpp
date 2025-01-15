#pragma once

#include <scriptos/std/types.hpp>

#define INT(N) asm volatile("int $" #N)

void outb(u16 port, u8 value);
u8 inb(u16 port);

void outw(u16 port, u16 value);
u16 inw(u16 port);

void io_wait();

void lidt(const struct IDT_Descriptor &idt);
void cli();
void sti();
void invlpg(void *address);
void loop();
