#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/tss.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/util.hpp>

GDT_Entry::GDT_Entry(u32 base, u32 limit, u8 access, u8 flags)
{
    BaseLo = base & 0xffff;
    BaseMi = (base >> 16) & 0xff;
    BaseHi = (base >> 24) & 0xff;
    LimitLo = limit & 0xffff;
    Flags_LimitHi = ((limit >> 16) & 0x0f) | (flags << 4);
    Access = access | 0b00000001;
}

static GDT_Entry gdt[6] __attribute__((aligned(0x1000)));
static TSS tss;

extern "C" void LoadGDT(GDT_Descriptor *descriptor, u16 code_segment, u16 data_segment);

void InitGDT()
{
    gdt[0] = {0, 0, 0, 0};                                                                                                                                          // null descriptor
    gdt[1] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Code_Segment | GDT_Access_Code_Readable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K};  // kernel code segment
    gdt[2] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Data_Segment | GDT_Access_Data_Writeable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K}; // kernel data segment
    gdt[3] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring3 | GDT_Access_Code_Segment | GDT_Access_Code_Readable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K};  // user code segment
    gdt[4] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring3 | GDT_Access_Data_Segment | GDT_Access_Data_Writeable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K}; // user data segment
    gdt[5] = {(uptr)&tss, sizeof(TSS) - 1, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Task_Segment, GDT_Flags_32Bit | GDT_Flags_Granularity_4K};            // task state segment

    memset(&tss, 0, sizeof(TSS));
    tss.SS0 = GDT_DATA_SEGMENT;
    tss.ESP0 = (uptr)TSS_END;
    tss.IOPB = sizeof(TSS);

    GDT_Descriptor desc{sizeof(gdt) - 1, gdt};
    LoadGDT(&desc, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}
