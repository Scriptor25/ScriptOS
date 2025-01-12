#include <scriptos/assert.hpp>
#include <scriptos/print.hpp>

void __assert(bool x, cstr exp, cstr file, usize line)
{
    if (!x)
        printf("%s:%u: assertion failed: '%s'", file, line, exp);
}
