#pragma once

#include <scriptos/graphics.h>
#include <scriptos/paging.h>

namespace kernel
{
    extern struct KernelInstance
    {
        PageFrameAllocator* Allocator;
        BasicRenderer* Renderer;
    } Instance;
}
