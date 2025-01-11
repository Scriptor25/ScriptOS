#include <scriptos/interrupts.hpp>
#include <scriptos/io.hpp>
#include <scriptos/panic.hpp>
#include <scriptos/print.hpp>

__attribute__((interrupt)) void DE_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Division Error");
}

__attribute__((interrupt)) void DB_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Debug");
}

__attribute__((interrupt)) void BP_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Breakpoint");
}

__attribute__((interrupt)) void OF_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Overflow");
}

__attribute__((interrupt)) void BR_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Bound Range Exceeded");
}

__attribute__((interrupt)) void UD_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Invalid Opcode");
}

__attribute__((interrupt)) void NM_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Device Not Available");
}

__attribute__((interrupt)) void DF_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Double Fault");
}

__attribute__((interrupt)) void TS_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Invalid TSS");
}

__attribute__((interrupt)) void NP_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Segment Not Present");
}

__attribute__((interrupt)) void SS_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Stack-Segment Fault");
}

__attribute__((interrupt)) void GP_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("General Protection Fault");
}

__attribute__((interrupt)) void PF_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Page Fault");
}

__attribute__((interrupt)) void MF_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("x87 Floating-Point Exception");
}

__attribute__((interrupt)) void AC_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Alignment Check");
}

__attribute__((interrupt)) void MC_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Machine Check");
}

__attribute__((interrupt)) void XM_XF_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("SIMD Floating-Point Exception");
}

__attribute__((interrupt)) void VE_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Virtualization Exception");
}

__attribute__((interrupt)) void CP_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Control Protection Exception");
}

__attribute__((interrupt)) void HV_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Hypervisor Injection Exception");
}

__attribute__((interrupt)) void VC_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("VMM Communication Exception");
}

__attribute__((interrupt)) void SX_Handler(interrupt_frame *frame)
{
    (void)frame;

    Panic("Security Exception");
}

__attribute__((interrupt)) void Keyboard_Handler(interrupt_frame *frame)
{
    (void)frame;

    print("hi");

    auto scancode = inb(0x60);
    PIC_EndMaster();
}

void RemapPIC()
{
    auto a1 = inb(PIC1_DATA);
    io_wait();
    auto a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}

void PIC_EndMaster()
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave()
{
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
