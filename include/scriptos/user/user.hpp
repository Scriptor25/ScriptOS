#pragma once

#include <scriptos/std/types.hpp>

/**
 * Call a kernel function from user mode. This will jump to kernel mode, execute the task and then jump back.
 */
extern "C" void syscall(...);

/**
 * User mode entry point.
 */
extern "C" void user_main();

/**
 * Print a null-terminated string to the screen.
 */
void print(cstr string);
/**
 * Print a null-terminated string to the screen.
 */
void wprint(cstr string);
/**
 * Print a string to the screen. The function prints the string until the count or a null-terminator is reached.
 */
void printn(cstr string, usize count);
/**
 * Print a string to the screen. The function prints the string until the count or a null-terminator is reached.
 */
void wprintn(cstr string, usize count);
/**
 * Print a formatted null-terminated string to the screen.
 */
void printf(cstr format, ...);
/**
 * Print a formatted null-terminated string to the screen.
 */
void wprintf(cstr format, ...);
