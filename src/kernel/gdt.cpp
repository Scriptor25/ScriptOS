#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/tss.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/util.hpp>

GDT::Entry::Entry(u32 base, u32 limit, u8 access, u8 flags)
    : LimitLo(limit & 0xffff),
      BaseLo(base & 0xffff),
      BaseMi((base >> 16) & 0xff),
      Access(access | 0b00000001),
      Flags_LimitHi(((limit >> 16) & 0x0f) | (flags << 4)),
      BaseHi((base >> 24) & 0xff)
{
}

static GDT::Entry gdt[6] __attribute__((aligned(0x1000)));
static TSS tss;

void GDT::Initialize(void *kernel_stack)
{
    gdt[0] = {0, 0, 0, 0};                                                                                                                             // null descriptor
    gdt[1] = {0, 0xfffff, Access_Present | Access_Ring0 | Access_Code_Segment | Access_Code_Readable, Flags_32Bit | Flags_Granularity_4K};             // kernel code segment
    gdt[2] = {0, 0xfffff, Access_Present | Access_Ring0 | Access_Data_Segment | Access_Data_Writeable, Flags_32Bit | Flags_Granularity_4K};            // kernel data segment
    gdt[3] = {0, 0xfffff, Access_Present | Access_Ring3 | Access_Code_Segment | Access_Code_Readable, Flags_32Bit | Flags_Granularity_4K};             // user code segment
    gdt[4] = {0, 0xfffff, Access_Present | Access_Ring3 | Access_Data_Segment | Access_Data_Writeable, Flags_32Bit | Flags_Granularity_4K};            // user data segment
    gdt[5] = {reinterpret_cast<uptr>(&tss), sizeof(TSS) - 1, Access_Present | Access_Ring0 | Access_Task_Segment, Flags_32Bit | Flags_Granularity_1B}; // task state segment

    memset(&tss, 0, sizeof(TSS));
    tss.IOPB = sizeof(TSS);
    tss.SS0 = GDT_DATA_SEGMENT;
    tss.ESP0 = reinterpret_cast<uptr>(kernel_stack);

    Descriptor desc{sizeof(gdt) - 1, gdt};
    LoadGDT(&desc, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}
