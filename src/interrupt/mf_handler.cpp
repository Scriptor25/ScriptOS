#include <scriptos/interrupt.h>

INTERRUPT void interrupt::MF_Handler(StackFrame* stack_frame)
{
    Panic("x87 FPU Floating-Point Error (Math Fault) (%02X:%016X, %02X:%016X)",
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
