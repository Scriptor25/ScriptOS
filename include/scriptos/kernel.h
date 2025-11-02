#pragma once

#include <scriptos/graphics.h>
#include <scriptos/paging.h>

namespace kernel
{
    extern struct KernelInstance
    {
        paging::PageFrameAllocator* Allocator;
        graphics::Renderer* Renderer;
    } Instance;
}
