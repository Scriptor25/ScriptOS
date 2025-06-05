#pragma once

namespace fpu
{
    void Initialize();

    extern "C" void __enable_fpu();
}
