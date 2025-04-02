#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

#define USER_TEXT __attribute__((section(".user_text")))
#define USER_RODATA __attribute__((section(".user_rodata")))
#define USER_DATA __attribute__((section(".user_data")))
#define USER_BSS __attribute__((section(".user_bss")))

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
extern "C" NORET USER_TEXT void user_main();

/**
 * Print a null-terminated string to the screen.
 */
USER_TEXT void user_print(cstr string);
/**
 * Print a null-terminated string to the screen.
 */
USER_TEXT void user_wprint(cstr string);
/**
 * Print a string to the screen. The function prints the string until the count or a null-terminator is reached.
 */
USER_TEXT void user_printn(cstr string, usize count);
/**
 * Print a string to the screen. The function prints the string until the count or a null-terminator is reached.
 */
USER_TEXT void user_wprintn(cstr string, usize count);
/**
 * Print a formatted null-terminated string to the screen.
 */
USER_TEXT void user_printf(cstr format, ...);
/**
 * Print a formatted null-terminated string to the screen.
 */
USER_TEXT void user_wprintf(cstr format, ...);
