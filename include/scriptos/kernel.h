#pragma once

#include <scriptos/graphics.h>
#include <scriptos/paging.h>

namespace kernel
{
    extern struct InstanceT
    {
        paging::PageFrameAllocator* Allocator;
        graphics::Renderer* Renderer;
    } Instance;
}
