#pragma once

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

void PIC_Remap();
void PIC_EndMaster();
void PIC_EndSlave();

void PIC_Mask_All();
void PIC_Enable_PS2_1();
void PIC_Disable_PS2_1();
