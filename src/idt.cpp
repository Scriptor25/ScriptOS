#include <scriptos/idt.h>
#include <scriptos/interrupt.h>
#include <scriptos/memory.h>

extern "C" void __load_idt(const kernel::InterruptDescriptor* descriptor);

kernel::GateDescriptor::GateDescriptor(
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

__attribute__((aligned(0x10))) static kernel::GateDescriptor entries[256];

void kernel::InitializeIDT()
{
    kernel::Fill(entries, 0, sizeof(entries));

    entries[0x00] = { reinterpret_cast<uptr>(__de_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x01] = { reinterpret_cast<uptr>(__db_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x02] = { reinterpret_cast<uptr>(__ni_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x03] = { reinterpret_cast<uptr>(__bp_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x04] = { reinterpret_cast<uptr>(__of_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x05] = { reinterpret_cast<uptr>(__br_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x06] = { reinterpret_cast<uptr>(__ud_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x07] = { reinterpret_cast<uptr>(__nm_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x08] = { reinterpret_cast<uptr>(__df_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0A] = { reinterpret_cast<uptr>(__ts_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0B] = { reinterpret_cast<uptr>(__np_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0C] = { reinterpret_cast<uptr>(__ss_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0D] = { reinterpret_cast<uptr>(__gp_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x0E] = { reinterpret_cast<uptr>(__pf_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x10] = { reinterpret_cast<uptr>(__mf_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x11] = { reinterpret_cast<uptr>(__ac_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x12] = { reinterpret_cast<uptr>(__mc_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x13] = { reinterpret_cast<uptr>(__xm_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x14] = { reinterpret_cast<uptr>(__ve_proxy), 0x0008, 0b000, 0xE, 0b00 };
    entries[0x15] = { reinterpret_cast<uptr>(__cp_proxy), 0x0008, 0b000, 0xF, 0b00 };

    entries[0x20] = { reinterpret_cast<uptr>(__irq0_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x21] = { reinterpret_cast<uptr>(__irq1_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x22] = { reinterpret_cast<uptr>(__irq2_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x23] = { reinterpret_cast<uptr>(__irq3_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x24] = { reinterpret_cast<uptr>(__irq4_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x25] = { reinterpret_cast<uptr>(__irq5_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x26] = { reinterpret_cast<uptr>(__irq6_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x27] = { reinterpret_cast<uptr>(__irq7_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x28] = { reinterpret_cast<uptr>(__irq8_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x29] = { reinterpret_cast<uptr>(__irq9_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2A] = { reinterpret_cast<uptr>(__irq10_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2B] = { reinterpret_cast<uptr>(__irq11_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2C] = { reinterpret_cast<uptr>(__irq12_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2D] = { reinterpret_cast<uptr>(__irq13_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2E] = { reinterpret_cast<uptr>(__irq14_proxy), 0x0008, 0b000, 0xF, 0b00 };
    entries[0x2F] = { reinterpret_cast<uptr>(__irq15_proxy), 0x0008, 0b000, 0xF, 0b00 };

    entries[0x69] = { reinterpret_cast<uptr>(__ke_proxy), 0x0008, 0b000, 0xE, 0b00 };

    const InterruptDescriptor descriptor = {
        static_cast<u16>(sizeof(entries) - 1),
        entries,
    };

    __load_idt(&descriptor);
}
