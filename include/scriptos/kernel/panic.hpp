#pragma once

#include <scriptos/std/types.hpp>

/**
 * Make the kernel completely panic
 */
__attribute__((noreturn)) void Panic(cstr format, ...);
