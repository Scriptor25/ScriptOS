#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/kernel/ps2.hpp>

void PS2_Enable_Keyboard_Interrupt()
{
    asm volatile("cli");

    outb(PIC1_DATA, 0b11111101);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    asm volatile("sti");
}

void PS2_Disable_Keyboard_Interrupt()
{
    asm volatile("cli");

    outb(PIC1_DATA, 0b11111111);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    asm volatile("sti");
}

u8 PS2_Keyboard_Command(u8 cmd)
{
    u8 response;
    do
    {
        outb(PS2_IO_DATA, cmd);
        io_wait();
        response = inb(PS2_IO_DATA);
        io_wait();
    } while (response == PS2_KEYBOARD_RESPONSE_RESEND);
    return response;
}

u8 PS2_Keyboard_Command(u8 cmd, u8 data)
{
    u8 response;
    do
    {
        outb(PS2_IO_DATA, cmd);
        io_wait();
        outb(PS2_IO_DATA, data);
        io_wait();
        response = inb(PS2_IO_DATA);
        io_wait();
    } while (response == PS2_KEYBOARD_RESPONSE_RESEND);
    return response;
}

u8 PS2_Keyboard_Command(u8 cmd, u8 *data, usize count)
{
    u8 response;
    do
    {
        outb(PS2_IO_DATA, cmd);
        io_wait();
        for (usize i = 0; i < count; ++i)
        {
            outb(PS2_IO_DATA, data[i]);
            io_wait();
        }
        response = inb(PS2_IO_DATA);
        io_wait();
    } while (response == PS2_KEYBOARD_RESPONSE_RESEND);
    return response;
}

bool PS2_SetLEDs(u8 state)
{
    return PS2_Keyboard_Command(PS2_KEYBOARD_CMD_SET_LEDS, state) == PS2_KEYBOARD_RESPONSE_ACK;
}

bool PS2_Echo()
{
    return PS2_Keyboard_Command(PS2_KEYBOARD_CMD_ECHO) == PS2_KEYBOARD_RESPONSE_ECHO;
}

bool PS2_SetCurrentScancodeSet(u8 value)
{
    return PS2_Keyboard_Command(PS2_KEYBOARD_CMD_SCANCODE_SET, value) == PS2_KEYBOARD_RESPONSE_ACK;
}

u8 PS2_GetCurrentScancodeSet()
{
    if (PS2_Keyboard_Command(PS2_KEYBOARD_CMD_SCANCODE_SET, PS2_KEYBOARD_SUBCMD_GET_SCANCODE_SET) != PS2_KEYBOARD_RESPONSE_ACK)
        return 0;
    auto scancode_set = inb(PS2_IO_DATA);
    io_wait();
    switch (scancode_set)
    {
    case 0x43:
        return 1;
    case 0x41:
        return 2;
    case 0x3f:
        return 3;
    default:
        return 0;
    }
}

u16 PS2_IdentifyKeyboard()
{
    if (PS2_Keyboard_Command(PS2_KEYBOARD_CMD_IDENTIFY) != PS2_KEYBOARD_RESPONSE_ACK)
        return -1;
    auto b0 = inb(PS2_IO_DATA);
    io_wait();
    auto b1 = inb(PS2_IO_DATA);
    io_wait();
    return b1 << 8 | b0;
}

bool PS2_EnableScanning()
{
    return PS2_Keyboard_Command(PS2_KEYBOARD_CMD_ENABLE_SCANNING) == PS2_KEYBOARD_RESPONSE_ACK;
}

bool PS2_DisableScanning()
{
    return PS2_Keyboard_Command(PS2_KEYBOARD_CMD_DISABLE_SCANNING) == PS2_KEYBOARD_RESPONSE_ACK;
}
