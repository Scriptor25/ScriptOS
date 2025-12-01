#pragma once

#include <scriptos/types.h>

constexpr u16 PIC1      = 0x0020;
constexpr u16 PIC1_CMD  = PIC1;
constexpr u16 PIC1_DATA = PIC1 + 1;

constexpr u16 PIC2      = 0x00A0;
constexpr u16 PIC2_CMD  = PIC2;
constexpr u16 PIC2_DATA = PIC2 + 1;

constexpr u8 PIC_EOI = 0x20;

constexpr u8 ICW1_ICW4      = 0x01;
constexpr u8 ICW1_SINGLE    = 0x02;
constexpr u8 ICW1_INTERVAL4 = 0x04;
constexpr u8 ICW1_LEVEL     = 0x08;
constexpr u8 ICW1_INIT      = 0x10;

constexpr u8 ICW4_8086       = 0x01;
constexpr u8 ICW4_AUTO       = 0x02;
constexpr u8 ICW4_BUF_SLAVE  = 0x08;
constexpr u8 ICW4_BUF_MASTER = 0x0C;
constexpr u8 ICW4_SFNM       = 0x10;

constexpr u8 CASCADE_IRQ = 2;

namespace kernel
{
    void RemapPic(
        int offset1,
        int offset2);

    void DisablePic();

    void SendPicEoi(u8 irq);

    void SetPicMask(u8 irq);
    void ClearPicMask(u8 irq);
}
