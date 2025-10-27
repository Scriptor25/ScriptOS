#include <scriptos/interrupt.h>

extern "C" void NP_Handler(interrupt::StackFrameError* stack_frame)
{
    auto external = stack_frame->error_code & 0b1;
    auto table = (stack_frame->error_code >> 1) & 0b11;
    auto index = (stack_frame->error_code >> 3) & 0x1fff;

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

    interrupt::Panic(
        false,
        "Segment Not Present %s%s [ %u ] (%02X:%016X, %02X:%016X)",
        external ? "EXT " : "",
        table_string,
        index,
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
