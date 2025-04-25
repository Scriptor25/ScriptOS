#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

/**
 * Jump from kernel to user mode.
 */
extern "C" NORET void jump_user(void* user_stack, void* user_main);
