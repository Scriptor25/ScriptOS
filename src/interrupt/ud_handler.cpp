#include <scriptos/interrupt.h>

extern "C" void __ud_handler(interrupt::StackFrame* stack_frame)
{
    interrupt::Panic(
        stack_frame,
        false,
        "Invalid Opcode (Undefined Opcode) (%02X:%016X, %02X:%016X)",
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
