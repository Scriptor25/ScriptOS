#pragma once

#include <scriptos/types.h>

namespace memory
{
    void fill(void* dst, int value, usize count);
    void copy(void* dst, const void* src, usize count);
}
