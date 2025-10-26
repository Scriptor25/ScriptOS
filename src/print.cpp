#include <scriptos/print.h>
#include <scriptos/renderer.h>
#include <scriptos/serial.h>

static void kstdout(int c)
{
    serial::WriteDefault(c);
    if (KernelRenderer)
    {
        KernelRenderer->NextChar(c);
    }
}

unsigned kprintf(
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = vfkprintf(kstdout, format, ap);
    va_end(ap);
    return count;
}

unsigned vkprintf(
    cstr format,
    va_list ap)
{
    return vfkprintf(kstdout, format, ap);
}

void kflush()
{
    if (KernelRenderer)
    {
        KernelRenderer->SwapBuffers();
    }
}

unsigned fkprintf(
    out_stream stream,
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = vfkprintf(stream, format, ap);
    va_end(ap);
    return count;
}

void kprintmem(
    const void* buffer,
    usize buffer_length)
{
    fkprintmem(kstdout, buffer, buffer_length);
}
