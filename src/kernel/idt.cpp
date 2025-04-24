#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/std/memory.hpp>

IDT::Entry::Entry(u32 offset, u16 selector, u8 attributes)
  : OffsetLo(offset & 0xffff), Selector(selector), Attributes(attributes), OffsetHi((offset >> 16) & 0xffff)
{
}

static IDT::Descriptor descriptor;
static IDT::Entry entries[256] __attribute__((aligned(0x1000)));

void IDT::Initialize()
{
    descriptor.Size = sizeof(entries) - 1;
    descriptor.Offset = entries;

    memset(entries, 0, sizeof(entries));

    // hardware interrupts
    entries[0x00] = { reinterpret_cast<uptr>(DE_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x01] = { reinterpret_cast<uptr>(DB_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Trap_Gate };
    entries[0x02] = { reinterpret_cast<uptr>(NMI_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x03] = { reinterpret_cast<uptr>(BP_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Trap_Gate };
    entries[0x04] = { reinterpret_cast<uptr>(OF_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Trap_Gate };
    entries[0x05] = { reinterpret_cast<uptr>(BR_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x06] = { reinterpret_cast<uptr>(UD_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x07] = { reinterpret_cast<uptr>(NM_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x08] = { reinterpret_cast<uptr>(DF_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    // 0x09 - reserved for co-processor segment overrun
    entries[0x0a] = { reinterpret_cast<uptr>(TS_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x0b] = { reinterpret_cast<uptr>(NP_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x0c] = { reinterpret_cast<uptr>(SS_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x0d] = { reinterpret_cast<uptr>(GP_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x0e] = { reinterpret_cast<uptr>(PF_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    // 0x0f - reserved by intel
    entries[0x10] = { reinterpret_cast<uptr>(MF_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x11] = { reinterpret_cast<uptr>(AC_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x12] = { reinterpret_cast<uptr>(MC_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x13] = { reinterpret_cast<uptr>(XM_XF_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x14] = { reinterpret_cast<uptr>(VE_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x15] = { reinterpret_cast<uptr>(CP_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    // 0x16 - 0x1b - reserved for future exceptions
    entries[0x1c] = { reinterpret_cast<uptr>(HV_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x1d] = { reinterpret_cast<uptr>(VC_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };
    entries[0x1e] = { reinterpret_cast<uptr>(SX_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };

    // programmable interrupts
    entries[PIC_IRQ0] = { reinterpret_cast<uptr>(PIT_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };

    // user mode system call interrupt
    entries[0x80] = { reinterpret_cast<uptr>(SYS_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring3 | Attributes_32Bit_Interrupt_Gate };

    // unhandled interrupts
    for (usize i = 0; i < (sizeof(entries) / sizeof(entries[0])); ++i)
        if (!entries[i].OffsetHi && !entries[i].OffsetLo)
            entries[i] = { reinterpret_cast<uptr>(NI_Handler), GDT_CODE_SEGMENT, Attributes_Present | Attributes_Ring0 | Attributes_32Bit_Interrupt_Gate };

    LoadIDT(&descriptor);
}

void IDT::LoadIDT(const Descriptor* descriptor)
{
    asm volatile("lidt (%0)" : : "r"(descriptor));
}
