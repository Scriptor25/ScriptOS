#include <scriptos/interrupt.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

static void panic(cstr message)
{
    serial::Write(message);
    serial::Write("\r\n");

    for (;;)
        asm volatile("cli; hlt");
}

INTERRUPT void interrupt::DE_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Divide Error");
}

INTERRUPT void interrupt::DB_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Debug Exception");
}

INTERRUPT void interrupt::NI_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Non Maskable Interrupt ");
}

INTERRUPT void interrupt::BP_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Breakpoint");
}

INTERRUPT void interrupt::OF_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Overflow");
}

INTERRUPT void interrupt::BR_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("BOUND Range Exceeded");
}

INTERRUPT void interrupt::UD_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Invalid Opcode (Undefined Opcode)");
}

INTERRUPT void interrupt::NM_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Device Not Available (No Math Coprocessor)");
}

INTERRUPT void interrupt::DF_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Double Fault");
}

INTERRUPT void interrupt::TS_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Invalid TSS");
}

INTERRUPT void interrupt::NP_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Segment Not Present");
}

INTERRUPT void interrupt::SS_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Stack-Segment Fault");
}

INTERRUPT void interrupt::GP_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("General Protection");
}

INTERRUPT void interrupt::PF_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Page Fault");
}

INTERRUPT void interrupt::MF_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("x87 FPU Floating-Point Error (Math Fault)");
}

INTERRUPT void interrupt::AC_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Alignment Check");
}

INTERRUPT void interrupt::MC_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Machine Check");
}

INTERRUPT void interrupt::XM_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("SIMD Floating-Point Exception");
}

INTERRUPT void interrupt::VE_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Virtualization Exception");
}

INTERRUPT void interrupt::CP_Handler(StackFrame* stack_frame, u64 error_code)
{
    (void) stack_frame;
    (void) error_code;

    panic("Control Protection Exception");
}

INTERRUPT void interrupt::KE_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    panic("Kernel Exception");
}
