#include <scriptos/interrupt.h>

INTERRUPT void interrupt::NM_Handler(StackFrame* stack_frame)
{
    Panic("Device Not Available (No Math Coprocessor) (%02X:%016X, %02X:%016X)",
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
