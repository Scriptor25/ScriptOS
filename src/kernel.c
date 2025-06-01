#include <scriptos/efi.h>
#include <scriptos/multiboot2.h>
#include <scriptos/types.h>

typedef int bool;

#define false ((bool) 0)
#define true  ((bool) 1)

#define SERIAL_PORT_COM1 0x3f8

void outb(u16 port, u8 data)
{
    asm volatile("outb %0, %1" ::"a"(data), "Nd"(port));
}

u8 inb(u16 port)
{
    u8 data;
    asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

u8 serial_transmit_empty()
{
    return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial_write(char data)
{
    while (!serial_transmit_empty())
        ;

    outb(SERIAL_PORT_COM1, data);
}

void serial_write_string(cstr data)
{
    for (str ptr = (str) data; *ptr; ++ptr)
        serial_write(*ptr);
}

bool serial_initialize()
{
    outb(SERIAL_PORT_COM1 + 1, 0x00);
    outb(SERIAL_PORT_COM1 + 3, 0x80);
    outb(SERIAL_PORT_COM1 + 0, 0x03);
    outb(SERIAL_PORT_COM1 + 1, 0x00);
    outb(SERIAL_PORT_COM1 + 3, 0x03);
    outb(SERIAL_PORT_COM1 + 2, 0xC7);
    outb(SERIAL_PORT_COM1 + 4, 0x0B);
    outb(SERIAL_PORT_COM1 + 4, 0x1E);
    outb(SERIAL_PORT_COM1 + 0, 0xAE);

    if (inb(SERIAL_PORT_COM1 + 0) != 0xAE)
        return false;

    outb(SERIAL_PORT_COM1 + 4, 0x0F);
    return true;
}

__attribute__((sysv_abi)) void kernel_main(u32 magic, u64 info)
{
    serial_initialize();

    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || (info & 7))
    {
        serial_write_string("invalid bootloader magic or misaligned info structure\r\n");
        return;
    }

    serial_write_string("successfully loaded kernel\r\n");

    for (;;)
        ;
}
