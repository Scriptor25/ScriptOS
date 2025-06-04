#include <scriptos/gdt.h>

gdt::SegmentDescriptor::SegmentDescriptor(u32 base, u32 limit, u8 access, u8 flags)
  : LimitLo(limit & 0xffff), BaseLo(base & 0xffffff), Access(access), LimitHi(limit & 0xf0000), Flags(flags & 0xf), BaseHi(base & 0xff000000)
{
}

gdt::SystemSegmentDescriptor::SystemSegmentDescriptor(u64 base, u32 limit, u8 access, u8 flags)
  : LimitLo(limit & 0xffff), BaseLo(base & 0xffffff), Access(access), LimitHi(limit & 0xf0000), Flags(flags & 0xf), BaseHi(base & 0xffff'ffff'ff00'0000)
{
}

static u8 entries[40];
static gdt::Descriptor desc;

void gdt::Initialize()
{
    usize offset = 0;
    offset = Insert(entries, offset, SegmentDescriptor(0x00000000, 0x00000, 0x00, 0x0));
    offset = Insert(entries, offset, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_0 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_LONG_MODE));
    offset = Insert(entries, offset, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_0 | GDT_CODE_DATA | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_32_BIT_SEGMENT));
    offset = Insert(entries, offset, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_3 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_LONG_MODE));
    offset = Insert(entries, offset, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_3 | GDT_CODE_DATA | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_32_BIT_SEGMENT));

    // TODO: tss
    // offset += insert(entries, offset, system_segment_descriptor(TODO, TODO, TODO, TODO));

    desc = { static_cast<u16>(offset - 1), entries };

    __load_gdt(&desc, 0x08, 0x10);
}

usize gdt::Insert(void* buffer, usize offset, const SegmentDescriptor& desc)
{
    auto dst = reinterpret_cast<u64*>(reinterpret_cast<uptr>(buffer) + offset);
    dst[0] = desc.Value;
    return offset + 8;
}

usize gdt::Insert(void* buffer, usize offset, const SystemSegmentDescriptor& desc)
{
    auto dst = reinterpret_cast<u64*>(reinterpret_cast<uptr>(buffer) + offset);
    dst[0] = desc.ValueLo;
    dst[1] = desc.ValueHi;
    return offset + 16;
}
