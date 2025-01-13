#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/std/print.hpp>

void Panic(cstr format, ...)
{
    auto &graphics = Graphics::GetInstance();
    graphics.DrawColorTest();
    graphics.Reset();
    graphics.SetCharColor(0xffffffff);

    print("Kernel Panic");
    putchar('\n');
    putchar('\n');
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    putchar('\n');

    graphics.SwapBuffers();
    asm volatile(
        "cli;"
        "jmp .;");
}
