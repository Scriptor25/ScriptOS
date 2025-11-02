#include <scriptos/graphics.h>
#include <scriptos/kernel.h>
#include <scriptos/print.h>
#include <scriptos/serial.h>

void kstdout(int c)
{
    serial::WriteDefault(c);

    if (kernel::Instance.Renderer)
    {
        kernel::Instance.Renderer->PushChar(c);
    }
}

void kflush()
{
    if (kernel::Instance.Renderer)
    {
        kernel::Instance.Renderer->SwapBuffers();
    }
}

void kputc(int c)
{
    kstdout(c);
}

void kputs(cstr s)
{
    while (*s)
    {
        kstdout(*s++);
    }
}

void fkputc(
    out_stream stream,
    int c)
{
    stream(c);
}

void fkputs(
    out_stream stream,
    cstr s)
{
    while (*s)
    {
        stream(*s++);
    }
}

int kprintf(
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = vfkprintf(kstdout, format, ap);
    va_end(ap);
    return count;
}

int vkprintf(
    cstr format,
    va_list ap)
{
    return vfkprintf(kstdout, format, ap);
}

int fkprintf(
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
