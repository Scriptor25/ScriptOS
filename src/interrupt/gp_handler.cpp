#include <scriptos/interrupt.h>

INTERRUPT void interrupt::GP_Handler(StackFrame* stack_frame, u64 error_code)
{
    auto external = error_code & 0b1;
    auto table = (error_code >> 1) & 0b11;
    auto index = (error_code >> 3) & 0x1fff;

    cstr table_string;
    switch (table)
    {
    case 0b00:
        table_string = "GDT";
        break;
    case 0b01:
        table_string = "IDT";
        break;
    case 0b10:
        table_string = "LDT";
        break;
    case 0b11:
        table_string = "IDT";
        break;
    }

    Panic("General Protection Fault %s%s [ %u ] (%02X:%016X, %02X:%016X)",
          external ? "EXT " : "",
          table_string,
          index,
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
