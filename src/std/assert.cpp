#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/std/assert.hpp>
#include <scriptos/std/print.hpp>

void __assert(bool x, cstr exp, cstr file, usize line)
{
    if (x)
        return;

    Panic("%s:%u: assertion failed: %s", file, line, exp);
}
