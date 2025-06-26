#include <scriptos/interrupt.h>
#include <scriptos/print.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

void interrupt::Panic(
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    PrintV(format, ap);
    Print("\r\n");
    va_end(ap);

    Flush();

    for (;;)
        asm volatile("cli; hlt");
}
