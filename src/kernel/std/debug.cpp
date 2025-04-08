#include <scriptos/std/debug.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/stream.hpp>
#include <scriptos/std/vararg.hpp>

void debug(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);

    vfprintf(stderr, format, ap);
    fputs("\r\n", stderr);

    va_end(ap);
}
