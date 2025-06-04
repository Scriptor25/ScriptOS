#pragma once

#include <scriptos/types.h>

namespace memory
{
    void Fill(void* dst, int value, usize count);
    void Copy(void* dst, const void* src, usize count);
}
