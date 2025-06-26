#include <scriptos/print.h>
#include <scriptos/renderer.h>
#include <scriptos/serial.h>

static void default_stream(int c)
{
    serial::WriteDefault(c);
    if (KernelRenderer)
        KernelRenderer->NextChar(c);
}

unsigned Print(
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = SPrintV(default_stream, format, ap);
    va_end(ap);
    return count;
}

unsigned PrintV(
    cstr format,
    va_list ap)
{
    return SPrintV(default_stream, format, ap);
}

void Flush()
{
    if (KernelRenderer)
        KernelRenderer->SwapBuffers();
}

unsigned SPrint(
    out_stream stream,
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = SPrintV(stream, format, ap);
    va_end(ap);
    return count;
}
