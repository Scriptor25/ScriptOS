#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/std/print.hpp>

NORET void Panic(cstr format, ...)
{
    CLI();

    auto &graphics = Graphics::GetKernelInstance();
    graphics.SetFGColor(0xffffffff);
    graphics.SetBGColor(0x00000000);
    graphics.Reset();
    graphics.ClearRainbow();

    puts("Kernel Panic\n\n");
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    putc('\n');

    graphics.SwapBuffers();

    HLT();
    LOOP();

    for (;;)
        ;
}
