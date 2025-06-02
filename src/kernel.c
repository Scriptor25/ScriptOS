#include <limine.h>
#include <scriptos/efi.h>
#include <scriptos/types.h>

#define LIMINE_REQUEST       __attribute__((used, section(".limine_requests"))) static volatile
#define LIMINE_REQUEST_START __attribute__((used, section(".limine_requests_start"))) static volatile
#define LIMINE_REQUEST_END   __attribute__((used, section(".limine_requests_end"))) static volatile

#define NORETURN __attribute__((noreturn))

#define NULL  ((void*) 0)
#define false ((bool) 0)
#define true  ((bool) 1)

#define SERIAL_PORT_COM1 0x3f8

LIMINE_REQUEST_START LIMINE_REQUESTS_START_MARKER;
LIMINE_REQUEST LIMINE_BASE_REVISION(3);
LIMINE_REQUEST struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};
LIMINE_REQUEST_END LIMINE_REQUESTS_END_MARKER;

typedef int bool;

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

NORETURN static void hcf(void)
{
    for (;;)
        asm volatile("hlt");
}

NORETURN void kmain(void)
{
    serial_initialize();

    if (!LIMINE_BASE_REVISION_SUPPORTED)
    {
        serial_write_string("limine base revision not supported\r\n");
        hcf();
    }

    if (!framebuffer_request.response || !framebuffer_request.response->framebuffer_count)
    {
        serial_write_string("no framebuffer response or no framebuffers\r\n");
        hcf();
    }

    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

    for (u64 i = 0; i < 100; ++i)
    {
        volatile u32* fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }

    serial_write_string("successfully loaded kernel\r\n");
    hcf();
}
