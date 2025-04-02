#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

#define USERFN __attribute__((section(".user_text")))

/**
 * Jump from kernel to user mode.
 */
extern "C" NORET void jump_user_main(void *user_stack);

/**
 * Call a kernel function from user mode. This will jump to kernel mode, execute the task and then jump back.
 */
extern "C" void syscall(...);

/**
 * User mode entry point.
 */
extern "C" NORET USERFN void user_main();

/**
 * Print a null-terminated string to the screen.
 */
USERFN void user_print(cstr string);
/**
 * Print a null-terminated string to the screen.
 */
USERFN void user_wprint(cstr string);
/**
 * Print a string to the screen. The function prints the string until the count or a null-terminator is reached.
 */
USERFN void user_printn(cstr string, usize count);
/**
 * Print a string to the screen. The function prints the string until the count or a null-terminator is reached.
 */
USERFN void user_wprintn(cstr string, usize count);
/**
 * Print a formatted null-terminated string to the screen.
 */
USERFN void user_printf(cstr format, ...);
/**
 * Print a formatted null-terminated string to the screen.
 */
USERFN void user_wprintf(cstr format, ...);
