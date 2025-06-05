#include <scriptos/interrupt.h>

INTERRUPT void interrupt::CP_Handler(StackFrame* stack_frame, u64 error_code)
{
    Panic("Control Protection Exception %016X (%02X:%016X, %02X:%016X)",
          error_code,
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
