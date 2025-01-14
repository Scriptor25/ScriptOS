#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/std/print.hpp>

__attribute__((interrupt)) void Unhandled(interrupt_frame *frame)
{
    Panic("Unhandled Interrupt\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void DE_Handler(interrupt_frame *frame)
{
    Panic("Division Error\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void DB_Handler(interrupt_frame *frame)
{
    Panic("Debug\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void BP_Handler(interrupt_frame *frame)
{
    Panic("Breakpoint\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void OF_Handler(interrupt_frame *frame)
{
    Panic("Overflow\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void BR_Handler(interrupt_frame *frame)
{
    Panic("Bound Range Exceeded\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void UD_Handler(interrupt_frame *frame)
{
    Panic("Invalid Opcode\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void NM_Handler(interrupt_frame *frame)
{
    Panic("Device Not Available\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void DF_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Double Fault\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

__attribute__((interrupt)) void TS_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("Invalid TSS\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

__attribute__((interrupt)) void NP_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("Segment Not Present\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

__attribute__((interrupt)) void SS_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("Stack-Segment Fault\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

__attribute__((interrupt)) void GP_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("General Protection Fault\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

__attribute__((interrupt)) void PF_Handler(interrupt_frame *frame, u32 code)
{
    auto p = code & 0b1;
    auto w = (code >> 1) & 0b1;
    auto u = (code >> 2) & 0b1;
    auto r = (code >> 3) & 0b1;
    auto i = (code >> 4) & 0b1;
    auto pk = (code >> 5) & 0b1;
    auto ss = (code >> 6) & 0b1;
    auto sgx = (code >> 15) & 0b1;

    Panic("Page Fault\n%04x:%p\np=%u, w=%u, u=%u, r=%u, i=%u, pk=%u, ss=%u, sgx=%u", frame->CS, frame->IP, p, w, u, r, i, pk, ss, sgx);
}

__attribute__((interrupt)) void MF_Handler(interrupt_frame *frame)
{
    Panic("x87 Floating-Point Exception\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void AC_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Alignment Check\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

__attribute__((interrupt)) void MC_Handler(interrupt_frame *frame)
{
    Panic("Machine Check\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void XM_XF_Handler(interrupt_frame *frame)
{
    Panic("SIMD Floating-Point Exception\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void VE_Handler(interrupt_frame *frame)
{
    Panic("Virtualization Exception\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void CP_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Control Protection Exception\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

__attribute__((interrupt)) void HV_Handler(interrupt_frame *frame)
{
    Panic("Hypervisor Injection Exception\n%04x:%p", frame->CS, frame->IP);
}

__attribute__((interrupt)) void VC_Handler(interrupt_frame *frame, u32 code)
{
    Panic("VMM Communication Exception\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

__attribute__((interrupt)) void SX_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Security Exception\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}
