#include <scriptos/graphics.hpp>
#include <scriptos/io.hpp>
#include <scriptos/panic.hpp>
#include <scriptos/print.hpp>

void Panic(cstr format, ...)
{
    auto &graphics = Graphics::Get();
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
