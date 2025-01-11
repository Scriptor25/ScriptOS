#include <scriptos/gdt.hpp>

GDT_Entry::GDT_Entry(u32 base, u32 limit, u8 access, u8 flags)
{
    BaseLow = base & 0xffff;
    BaseMiddle = (base >> 16) & 0xff;
    BaseHigh = (base >> 24) & 0xff;
    LimitLow = limit & 0xffff;
    FlagsLimitHigh = ((limit >> 16) & 0x0f) | (flags << 4);
    Access = access;
}

static GDT_Entry GDT[3] __attribute__((aligned(0x1000)));

void InitGDT()
{
    GDT[0] = {0, 0, 0, 0};                                                                                                                                          // null
    GDT[1] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Code_Segment | GDT_Access_Code_Readable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K};  // code
    GDT[2] = {0, 0xfffff, GDT_Access_Present | GDT_Access_Ring0 | GDT_Access_Data_Segment | GDT_Access_Data_Writeable, GDT_Flags_32Bit | GDT_Flags_Granularity_4K}; // data

    GDT_Descriptor desc{sizeof(GDT) - 1, GDT};
    LoadGDT(&desc, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}
