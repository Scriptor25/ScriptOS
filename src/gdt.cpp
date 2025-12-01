#include <scriptos/gdt.h>
#include <scriptos/tss.h>
#include <scriptos/types.h>

extern "C" void __load_gdt(
    const kernel::GlobalDescriptor* descriptor,
    u16 code_segment,
    u16 data_segment);

kernel::GlobalSegmentDescriptor::GlobalSegmentDescriptor(
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

kernel::GlobalSystemSegmentDescriptor::GlobalSystemSegmentDescriptor(
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

void kernel::InitializeGDT()
{
    InsertGlobalSegmentDescriptor(entries, 0x00, GlobalSegmentDescriptor(0x00000000, 0x00000, 0x00, 0x0));
    InsertGlobalSegmentDescriptor(entries, 0x08, GlobalSegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_0 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_LONG_MODE));
    InsertGlobalSegmentDescriptor(entries, 0x10, GlobalSegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_0 | GDT_CODE_DATA | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_32_BIT_SEGMENT));
    InsertGlobalSegmentDescriptor(entries, 0x18, GlobalSegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_3 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_LONG_MODE));
    InsertGlobalSegmentDescriptor(entries, 0x20, GlobalSegmentDescriptor(0x00000000, 0xfffff, GDT_PRESENT | GDT_LEVEL_3 | GDT_CODE_DATA | GDT_READABLE_WRITEABLE, GDT_PAGE_GRANULARITY | GDT_32_BIT_SEGMENT));

    auto tss0_address = reinterpret_cast<uptr>(&kernel::TSS0);
    InsertGlobalSystemSegmentDescriptor(entries, 0x28, GlobalSystemSegmentDescriptor(tss0_address, sizeof(kernel::TaskStateSegment) - 1, GDT_PRESENT | GDT_LEVEL_0 | GDT_EXECUTABLE | GDT_ACCESSED, 0x0));

    const GlobalDescriptor descriptor = {
        static_cast<u16>(sizeof(entries) - 1),
        entries,
    };

    __load_gdt(&descriptor, 0x08, 0x10);
}

usize kernel::InsertGlobalSegmentDescriptor(
    void* buffer,
    usize offset,
    const GlobalSegmentDescriptor& descriptor)
{
    auto dst = reinterpret_cast<u64*>(reinterpret_cast<uptr>(buffer) + offset);
    dst[0]   = descriptor.Value;
    return offset + 8;
}

usize kernel::InsertGlobalSystemSegmentDescriptor(
    void* buffer,
    usize offset,
    const GlobalSystemSegmentDescriptor& descriptor)
{
    auto dst = reinterpret_cast<u64*>(reinterpret_cast<uptr>(buffer) + offset);
    dst[0]   = descriptor.ValueLo;
    dst[1]   = descriptor.ValueHi;
    return offset + 16;
}
