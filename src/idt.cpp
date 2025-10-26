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

    entries[0x00] = { reinterpret_cast<uptr>(interrupt::DE_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x01] = { reinterpret_cast<uptr>(interrupt::DB_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x02] = { reinterpret_cast<uptr>(interrupt::NI_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x03] = { reinterpret_cast<uptr>(interrupt::BP_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x04] = { reinterpret_cast<uptr>(interrupt::OF_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x05] = { reinterpret_cast<uptr>(interrupt::BR_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x06] = { reinterpret_cast<uptr>(interrupt::UD_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x07] = { reinterpret_cast<uptr>(interrupt::NM_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x08] = { reinterpret_cast<uptr>(interrupt::DF_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0A] = { reinterpret_cast<uptr>(interrupt::TS_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0B] = { reinterpret_cast<uptr>(interrupt::NP_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0C] = { reinterpret_cast<uptr>(interrupt::SS_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0D] = { reinterpret_cast<uptr>(interrupt::GP_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0E] = { reinterpret_cast<uptr>(interrupt::PF_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x10] = { reinterpret_cast<uptr>(interrupt::MF_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x11] = { reinterpret_cast<uptr>(interrupt::AC_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x12] = { reinterpret_cast<uptr>(interrupt::MC_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x13] = { reinterpret_cast<uptr>(interrupt::XM_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x14] = { reinterpret_cast<uptr>(interrupt::VE_Handler), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x15] = { reinterpret_cast<uptr>(interrupt::CP_Handler), 0x0008, 0b000, 0xF, 0b00 };

    entries[0x20] = { reinterpret_cast<uptr>(interrupt::IRQ0_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x21] = { reinterpret_cast<uptr>(interrupt::IRQ1_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x22] = { reinterpret_cast<uptr>(interrupt::IRQ2_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x23] = { reinterpret_cast<uptr>(interrupt::IRQ3_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x24] = { reinterpret_cast<uptr>(interrupt::IRQ4_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x25] = { reinterpret_cast<uptr>(interrupt::IRQ5_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x26] = { reinterpret_cast<uptr>(interrupt::IRQ6_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x27] = { reinterpret_cast<uptr>(interrupt::IRQ7_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x28] = { reinterpret_cast<uptr>(interrupt::IRQ8_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x29] = { reinterpret_cast<uptr>(interrupt::IRQ9_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2A] = { reinterpret_cast<uptr>(interrupt::IRQA_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2B] = { reinterpret_cast<uptr>(interrupt::IRQB_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2C] = { reinterpret_cast<uptr>(interrupt::IRQC_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2D] = { reinterpret_cast<uptr>(interrupt::IRQD_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2E] = { reinterpret_cast<uptr>(interrupt::IRQE_Handler), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2F] = { reinterpret_cast<uptr>(interrupt::IRQF_Handler), 0x0008, 0b000, 0xF, 0b00 };

    entries[0x69] = { reinterpret_cast<uptr>(interrupt::KE_Handler), 0x0008, 0b000, 0xE, 0b00 };

    const Descriptor descriptor = {
        static_cast<u16>(sizeof(entries) - 1),
        entries,
    };

    __load_idt(&descriptor);
}
