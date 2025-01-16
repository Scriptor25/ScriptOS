#pragma once

#include <scriptos/std/types.hpp>

#define USERFN __attribute__((section(".user_text")))

extern "C" void jump_user_main(void *user_stack);
extern "C" void syscall(...);

extern "C" USERFN void user_main();
USERFN void user_print(cstr string);
