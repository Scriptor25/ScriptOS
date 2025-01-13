#pragma once

#include <scriptos/std/types.hpp>

#define assert(EXP) __assert(EXP, #EXP, __FILE__, __LINE__)

void __assert(bool x, cstr exp, cstr file, usize line);
