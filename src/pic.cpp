#include <scriptos/io.h>
#include <scriptos/pic.h>

void pic::Remap(
    int offset1,
    int offset2)
{
    io::outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);

    io::wait();
    io::outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io::wait();
    io::outb(PIC1_DATA, offset1);
    io::wait();
    io::outb(PIC2_DATA, offset2);
    io::wait();
    io::outb(PIC1_DATA, 1 << CASCADE_IRQ);

    io::wait();
    io::outb(PIC2_DATA, 2);

    io::wait();

    io::outb(PIC1_DATA, ICW4_8086);

    io::wait();
    io::outb(PIC2_DATA, ICW4_8086);
    io::wait();

    io::outb(PIC1_DATA, 0);
    io::outb(PIC2_DATA, 0);
}

void pic::Disable()
{
    io::outb(PIC1_DATA, 0xFF);
    io::outb(PIC2_DATA, 0xFF);
}

void pic::SendEOI(u8 irq)
{
    if (irq >= 8)
    {
        io::outb(PIC2_CMD, PIC_EOI);
    }

    io::outb(PIC1_CMD, PIC_EOI);
}

void pic::SetMask(u8 irq)
{
    u16 port;
    u8 value;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = io::inb(port) | (1 << irq);
    io::outb(port, value);
}

void pic::ClearMask(u8 irq)
{
    u16 port;
    u8 value;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = io::inb(port) & ~(1 << irq);
    io::outb(port, value);
}
