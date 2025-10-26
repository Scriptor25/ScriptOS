#include <scriptos/interrupt.h>
#include <scriptos/print.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

void interrupt::Panic(
    bool serious,
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);

    if (serious)
    {
        vfkprintf(serial::WriteDefault, format, ap);
        fkprintf(serial::WriteDefault, "\r\n");
    }
    else
    {
        vkprintf(format, ap);
        kprintf("\r\n");
        kflush();
    }

    va_end(ap);

    asm volatile("cli");
    for (;;)
    {
        asm volatile("hlt");
    }
}
