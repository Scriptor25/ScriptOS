#include <scriptos/idt.h>
#include <scriptos/interrupt.h>
#include <scriptos/memory.h>

idt::GateDescriptor::GateDescriptor(
    u64 offset,
    u16 segment_selector,
    u8 interrupt_stack_table,
    u8 gate_type,
    u8 privilege_level)
    : OffsetLo(offset & 0xFFFF),
      SegmentSelector(segment_selector),
      InterruptStackTable(interrupt_stack_table),
      GateType(gate_type),
      PrivilegeLevel(privilege_level),
      Present(1),
      OffsetHi((offset & 0xFFFF'FFFF'FFFF'0000) >> 16)
{
}

__attribute__((aligned(0x10))) static idt::GateDescriptor entries[256];

void idt::Initialize()
{
    memory::Fill(entries, 0, sizeof(entries));

    entries[0x00] = { reinterpret_cast<uptr>(DE_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x01] = { reinterpret_cast<uptr>(DB_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x02] = { reinterpret_cast<uptr>(NI_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x03] = { reinterpret_cast<uptr>(BP_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x04] = { reinterpret_cast<uptr>(OF_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x05] = { reinterpret_cast<uptr>(BR_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x06] = { reinterpret_cast<uptr>(UD_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x07] = { reinterpret_cast<uptr>(NM_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x08] = { reinterpret_cast<uptr>(DF_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0A] = { reinterpret_cast<uptr>(TS_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0B] = { reinterpret_cast<uptr>(NP_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0C] = { reinterpret_cast<uptr>(SS_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0D] = { reinterpret_cast<uptr>(GP_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0E] = { reinterpret_cast<uptr>(PF_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x10] = { reinterpret_cast<uptr>(MF_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x11] = { reinterpret_cast<uptr>(AC_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x12] = { reinterpret_cast<uptr>(MC_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x13] = { reinterpret_cast<uptr>(XM_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x14] = { reinterpret_cast<uptr>(VE_Proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x15] = { reinterpret_cast<uptr>(CP_Proxy), 0x0008, 0b000, 0xF, 0b00 };

    entries[0x20] = { reinterpret_cast<uptr>(IRQ0_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x21] = { reinterpret_cast<uptr>(IRQ1_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x22] = { reinterpret_cast<uptr>(IRQ2_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x23] = { reinterpret_cast<uptr>(IRQ3_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x24] = { reinterpret_cast<uptr>(IRQ4_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x25] = { reinterpret_cast<uptr>(IRQ5_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x26] = { reinterpret_cast<uptr>(IRQ6_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x27] = { reinterpret_cast<uptr>(IRQ7_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x28] = { reinterpret_cast<uptr>(IRQ8_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x29] = { reinterpret_cast<uptr>(IRQ9_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2A] = { reinterpret_cast<uptr>(IRQA_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2B] = { reinterpret_cast<uptr>(IRQB_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2C] = { reinterpret_cast<uptr>(IRQC_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2D] = { reinterpret_cast<uptr>(IRQD_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2E] = { reinterpret_cast<uptr>(IRQE_Proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2F] = { reinterpret_cast<uptr>(IRQF_Proxy), 0x0008, 0b000, 0xF, 0b00 };

    entries[0x69] = { reinterpret_cast<uptr>(KE_Proxy), 0x0008, 0b000, 0xE, 0b00 };

    const Descriptor descriptor = {
        static_cast<u16>(sizeof(entries) - 1),
        entries,
    };

    __load_idt(&descriptor);
}
