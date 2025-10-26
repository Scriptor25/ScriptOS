#pragma once

#include <scriptos/types.h>

#define PIC1      0x0020
#define PIC1_CMD  PIC1
#define PIC1_DATA (PIC1 + 1)

#define PIC2      0x00A0
#define PIC2_CMD  PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

#define ICW1_ICW4      0x01
#define ICW1_SINGLE    0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL     0x08
#define ICW1_INIT      0x10

#define ICW4_8086       0x01
#define ICW4_AUTO       0x02
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM       0x10

#define CASCADE_IRQ 2

namespace pic
{
    void Remap(
        int offset1,
        int offset2);

    void Disable();

    void SendEOI(u8 irq);

    void SetMask(u8 irq);
    void ClearMask(u8 irq);
}
