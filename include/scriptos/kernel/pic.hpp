#pragma once

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

#define PIC_IRQ0 (PIC1_OFFSET + 0)
#define PIC_IRQ1 (PIC1_OFFSET + 1)
#define PIC_IRQ2 (PIC1_OFFSET + 2)
#define PIC_IRQ3 (PIC1_OFFSET + 3)
#define PIC_IRQ4 (PIC1_OFFSET + 4)
#define PIC_IRQ5 (PIC1_OFFSET + 5)
#define PIC_IRQ6 (PIC1_OFFSET + 6)
#define PIC_IRQ7 (PIC1_OFFSET + 7)
#define PIC_IRQ8 (PIC2_OFFSET + 0)
#define PIC_IRQ9 (PIC2_OFFSET + 1)
#define PIC_IRQ10 (PIC2_OFFSET + 2)
#define PIC_IRQ11 (PIC2_OFFSET + 3)
#define PIC_IRQ12 (PIC2_OFFSET + 4)
#define PIC_IRQ13 (PIC2_OFFSET + 5)
#define PIC_IRQ14 (PIC2_OFFSET + 6)
#define PIC_IRQ15 (PIC2_OFFSET + 7)

namespace PIC
{
    /**
     * Remap the interrupt offsets for the PIC.
     */
    void Remap(u8 offset_1, u8 offset_2);
    /**
     * Tell the PIC that the interrupt was handled.
     */
    void Send_EOI(u8 irq);
    /**
     * Disable the PIC by masking all data bits.
     */
    void Disable();
    /**
     * Enable an interrupt in the PIC by clearing a data bit.
     */
    void Clr_Mask(u8 irq);
    /**
     * Disable an interrupt in the PIC by setting a data bit.
     */
    void Set_Mask(u8 irq);
}
