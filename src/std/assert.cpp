#include <scriptos/std/assert.hpp>
#include <scriptos/std/print.hpp>

void __assert(bool x, cstr exp, cstr file, usize line)
{
    if (!x)
        printf("%s:%u: assertion failed: '%s'\n", file, line, exp);
}
