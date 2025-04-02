#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

/**
 * Make the kernel completely panic and stop execution.
 */
NORET void Panic(cstr format, ...);
