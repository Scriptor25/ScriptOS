#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

#define USERFN __attribute__((section(".user_text")))

extern "C" NORET void jump_user_main(void *user_stack);
extern "C" void syscall(...);

extern "C" NORET USERFN void user_main();

USERFN void user_print(cstr string);
USERFN void user_wprint(cstr string);
USERFN void user_printn(cstr string, usize count);
USERFN void user_wprintn(cstr string, usize count);
USERFN void user_printf(cstr format, ...);
USERFN void user_wprintf(cstr format, ...);
