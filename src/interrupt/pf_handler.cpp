#include <scriptos/interrupt.h>

INTERRUPT void interrupt::PF_Handler(StackFrame* stack_frame, u64 error_code)
{
    auto present = error_code & 0b1;
    auto read_write = (error_code >> 1) & 0b1;
    auto user = (error_code >> 2) & 0b1;
    auto reserved_write = (error_code >> 3) & 0b1;
    auto instruction_fetch = (error_code >> 4) & 0b1;
    auto protection_key = (error_code >> 5) & 0b1;
    auto shadow_stack = (error_code >> 6) & 0b1;
    auto software_guard_extension = (error_code >> 15) & 0b1;

    Panic("Page Fault "
          "[ ... ][ %u ][ ... ][ %u ][ %u ][ %u ][ %u ][ %u ][ %u ][ %u ] "
          "(%02X:%016X, %02X:%016X)",
          software_guard_extension,
          shadow_stack,
          protection_key,
          instruction_fetch,
          reserved_write,
          user,
          read_write,
          present,
          stack_frame->CS,
          stack_frame->IP,
          stack_frame->SS,
          stack_frame->SP);
}
