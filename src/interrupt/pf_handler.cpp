#include <scriptos/interrupt.h>

extern "C" void __pf_handler(interrupt::StackFrameError* stack_frame)
{
    auto present = stack_frame->error_code & 0b1;
    auto read_write = (stack_frame->error_code >> 1) & 0b1;
    auto user = (stack_frame->error_code >> 2) & 0b1;
    auto reserved_write = (stack_frame->error_code >> 3) & 0b1;
    auto instruction_fetch = (stack_frame->error_code >> 4) & 0b1;
    auto protection_key = (stack_frame->error_code >> 5) & 0b1;
    auto shadow_stack = (stack_frame->error_code >> 6) & 0b1;
    auto software_guard_extension = (stack_frame->error_code >> 15) & 0b1;

    u64 address;
    asm volatile("mov %%cr2, %0" : "=a"(address));

    interrupt::Panic(
        stack_frame,
        false,
        "Page Fault %016X "
        "[ ... ][ %u ][ ... ][ %u ][ %u ][ %u ][ %u ][ %u ][ %u ][ %u ] "
        "(%02X:%016X, %02X:%016X)",
        address,
        software_guard_extension,
        shadow_stack,
        protection_key,
        instruction_fetch,
        reserved_write,
        user,
        read_write,
        present,
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
