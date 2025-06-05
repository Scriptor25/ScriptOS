#include <scriptos/interrupt.h>

INTERRUPT void interrupt::DF_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) error_code;

    Panic("Double Fault (%02X:%016X, %02X:%016X)",
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
