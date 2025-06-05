#include <scriptos/interrupt.h>

INTERRUPT void interrupt::AC_Handler(StackFrame* stack_frame, u64 error_code)
{
    Panic("Alignment Check %016X (%02X:%016X, %02X:%016X)",
          error_code,
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
