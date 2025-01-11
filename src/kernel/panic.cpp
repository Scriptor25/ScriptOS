#include <scriptos/graphics.hpp>
#include <scriptos/panic.hpp>
#include <scriptos/print.hpp>

void Panic(cstr message)
{
    auto &graphics = Graphics::Get();
    graphics.DrawColorTest();
    graphics.Reset();
    graphics.SetCharColor(0xffffffff);

    print("Kernel Panic");
    putchar('\n');
    putchar('\n');
    print(message);
    putchar('\n');

    graphics.SwapBuffers();
    asm volatile(
        "cli;"
        "jmp .;");
}
