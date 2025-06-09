#include <scriptos/format.h>
#include <scriptos/interrupt.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

void interrupt::Panic(
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    PrintV(serial::WriteDefault, format, ap);
    Print(serial::WriteDefault, "\r\n");
    va_end(ap);

    for (;;)
        asm volatile("cli; hlt");
}
