#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pic.hpp>

void PIC_Remap()
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
    io_wait();
}

void PIC_EndMaster()
{
    outb(PIC1_COMMAND, PIC_EOI);
    io_wait();
}

void PIC_EndSlave()
{
    outb(PIC2_COMMAND, PIC_EOI);
    io_wait();
    outb(PIC1_COMMAND, PIC_EOI);
    io_wait();
}

void PIC_Mask_All()
{
    cli();

    outb(PIC1_DATA, 0b11111111);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    sti();
}

void PIC_Enable_PS2_1()
{
    cli();

    auto mask = inb(PIC1_DATA);
    io_wait();

    mask &= ~0b00000010;

    outb(PIC1_DATA, mask);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    sti();
}

void PIC_Disable_PS2_1()
{
    cli();

    auto mask = inb(PIC1_DATA);
    io_wait();

    mask |= 0b00000010;

    outb(PIC1_DATA, mask);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    sti();
}
