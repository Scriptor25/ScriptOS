#include <scriptos/gdt.hpp>

GDT_Entry::GDT_Entry(u32 base, u32 limit, u8 access, u8 flags)
{
    BaseLo = base & 0xffff;
    BaseMi = (base >> 16) & 0xff;
    BaseHi = (base >> 24) & 0xff;
    LimitLo = limit & 0xffff;
    Flags_LimitHi = ((limit >> 16) & 0x0f) | (flags << 4);
    Access = access;
}

static GDT_Entry gdt[3] __attribute__((aligned(0x1000)));

void InitGDT()
{
    gdt[0] = {0, 0, 0, 0};                                                                                                                                          // null
    gdt[1] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Code_Segment | GDT_Access_Code_Readable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K};  // code
    gdt[2] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Data_Segment | GDT_Access_Data_Writeable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K}; // data

    GDT_Descriptor desc{sizeof(gdt) - 1, gdt};
    LoadGDT(&desc, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}
