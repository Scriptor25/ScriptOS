#include <scriptos/interrupt.h>

INTERRUPT void interrupt::DE_Handler(StackFrame* stack_frame)
{
    Panic("Divide Error (%02X:%016X, %02X:%016X)",
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
