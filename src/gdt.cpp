#include <scriptos/gdt.h>
#include <scriptos/tss.h>
#include <scriptos/types.h>

gdt::SegmentDescriptor::SegmentDescriptor(
    u32 base,
    u32 limit,
    u8 access,
    u8 flags)
    : LimitLo(limit & 0xffff),
      BaseLo(base & 0xffffff),
      Access(access),
      LimitHi((limit & 0xf0000) >> 16),
      Flags(flags & 0xf),
      BaseHi((base & 0xff000000) >> 24)
{
}

gdt::SystemSegmentDescriptor::SystemSegmentDescriptor(
    u64 base,
    u32 limit,
    u8 access,
    u8 flags)
    : LimitLo(limit & 0xffff),
      BaseLo(base & 0xffffff),
      Access(access),
      LimitHi((limit & 0xf0000) >> 16),
      Flags(flags & 0xf),
      BaseHi((base & 0xffff'ffff'ff00'0000) >> 24)
{
}

__attribute__((aligned(0x10))) static u8 entries[0x38];

void gdt::Initialize()
{
    Insert(entries, 0x00, SegmentDescriptor(0x00000000, 0x00000, 0x00, 0x0));
    Insert(entries, 0x08, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_0 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_LONG_MODE));
    Insert(entries, 0x10, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_0 | GDT_CODE_DATA | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_32_BIT_SEGMENT));
    Insert(entries, 0x18, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_3 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_LONG_MODE));
    Insert(entries, 0x20, SegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_3 | GDT_CODE_DATA | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_32_BIT_SEGMENT));

    auto tss0_address = reinterpret_cast<uptr>(&tss::TSS0);
    Insert(entries, 0x28, SystemSegmentDescriptor(tss0_address, sizeof(tss::TaskStateSegment) - 1, GDT_PRESENT | GDT_LEVEL_0 | GDT_EXECUTABLE | GDT_ACCESSED, 0x0));

    const Descriptor descriptor = {
        static_cast<u16>(sizeof(entries) - 1),
        entries,
    };

    __load_gdt(&descriptor, 0x08, 0x10);
}

usize gdt::Insert(
    void* buffer,
    usize offset,
    const SegmentDescriptor& desc)
{
    auto dst = reinterpret_cast<u64*>(reinterpret_cast<uptr>(buffer) + offset);
    dst[0] = desc.Value;
    return offset + 8;
}

usize gdt::Insert(
    void* buffer,
    usize offset,
    const SystemSegmentDescriptor& desc)
{
    auto dst = reinterpret_cast<u64*>(reinterpret_cast<uptr>(buffer) + offset);
    dst[0] = desc.ValueLo;
    dst[1] = desc.ValueHi;
    return offset + 16;
}
