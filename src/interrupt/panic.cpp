#include <scriptos/asm.h>
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
        fkputs(serial::WriteDefault, "\r\n");
    }
    else
    {
        vkprintf(format, ap);
        kputs("\r\n");
        kflush();
    }

    va_end(ap);

    for (;;)
    {
        hlt();
    }
}
