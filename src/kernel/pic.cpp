#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pic.hpp>

void PIC_Remap(u8 offset_1, u8 offset_2)
{
    auto a1 = inb(PIC1_DATA);
    io_wait();
    auto a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, offset_1);
    io_wait();
    outb(PIC2_DATA, offset_2);
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
    outb(PIC2_DATA, a2);
}

void PIC_Send_EOI(u8 irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_Disable()
{
    outb(PIC1_DATA, 0b11111111);
    outb(PIC2_DATA, 0b11111111);
}

void PIC_Clr_Mask(u8 irq)
{
    u16 port;
    if (irq < 8)
        port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }

    outb(port, inb(port) & ~(1 << irq));
}

void PIC_Set_Mask(u8 irq)
{
    u16 port;
    if (irq < 8)
        port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }

    outb(port, inb(port) | (1 << irq));
}
