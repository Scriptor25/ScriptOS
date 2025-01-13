#pragma once

#include <scriptos/std/types.hpp>

#define INT(N) asm volatile("int $" #N)

void outb(u16 port, u8 value);
u8 inb(u16 port);

void io_wait();
