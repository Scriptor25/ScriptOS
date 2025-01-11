#include <scriptos/gdt.hpp>
#include <scriptos/idt.hpp>
#include <scriptos/interrupts.hpp>
#include <scriptos/pfa.hpp>

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
    idt.Ptr = (IDT_Entry *)PageFrameAllocator::Get().RequestEmptyPage();

    idt.Ptr[0x0e] = {(uptr)PageFault_Handler, GDT_CODE_SEGMENT, IDT_Attributes_Present | IDT_Attributes_Ring0 | IDT_Attributes_32Bit_Interrupt_Gate};

    asm volatile("lidt %0" : : "m"(idt));
}
