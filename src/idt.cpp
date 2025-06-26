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

    entries[0x69] = { reinterpret_cast<uptr>(interrupt::KE_Handler), 0x0008, 0b000, 0xE, 0b00 };

    const Descriptor descriptor = {
        static_cast<u16>(sizeof(entries) - 1),
        entries,
    };

    __load_idt(&descriptor);
}
