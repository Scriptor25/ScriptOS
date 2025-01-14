#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/pfa.hpp>

IDT_Entry::IDT_Entry(u32 offset, u16 selector, u8 attributes)
{
    OffsetLo = offset & 0xffff;
    OffsetHi = (offset >> 16) & 0xffff;
    Selector = selector;
    Attributes = attributes;
    Reserved = 0;
}

static IDT_Descriptor idt;

void InitIDT()
{
    idt.Limit = 0x800;
    idt.Ptr = (IDT_Entry *)PageFrameAllocator::GetInstance().RequestEmptyPage();

    idt.Ptr[0x00] = {(uptr)DE_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x01] = {(uptr)DB_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Trap_Gate};
    idt.Ptr[0x03] = {(uptr)BP_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Trap_Gate};
    idt.Ptr[0x04] = {(uptr)OF_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Trap_Gate};
    idt.Ptr[0x05] = {(uptr)BR_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x06] = {(uptr)UD_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x07] = {(uptr)NM_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x08] = {(uptr)DF_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x0a] = {(uptr)TS_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x0b] = {(uptr)NP_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x0c] = {(uptr)SS_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x0d] = {(uptr)GP_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x0e] = {(uptr)PF_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x10] = {(uptr)MF_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x11] = {(uptr)AC_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x12] = {(uptr)MC_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x13] = {(uptr)XM_XF_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x14] = {(uptr)VE_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x15] = {(uptr)CP_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x1c] = {(uptr)HV_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x1d] = {(uptr)VC_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};
    idt.Ptr[0x1e] = {(uptr)SX_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};

    for (usize i = 0; i < idt.Limit; i++)
        if (!(idt.Ptr[i].Attributes & IDT_Attributes_Present))
            idt.Ptr[i] = {(uptr)Unhandled, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};

    asm volatile("lidt %0" : : "m"(idt));
}
