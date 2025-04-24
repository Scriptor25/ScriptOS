#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/tss.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/util.hpp>

GDT::Entry::Entry(u32 base, u32 limit, u8 access, u8 flags)
  : LimitLo(limit & 0xffff), BaseLo(base & 0xffff), BaseMi((base >> 16) & 0xff), Access(access | 0b00000001), LimitHi(((limit >> 16) & 0x0f)), Flags(flags & 0x0f), BaseHi((base >> 24) & 0xff)
{
}

static GDT::Descriptor descriptor;
static GDT::Entry entries[6] __attribute__((aligned(0x1000)));

static TSS tss;

void GDT::Initialize(void* kernel_stack)
{
    descriptor.Size = sizeof(entries) - 1;
    descriptor.Offset = entries;

    entries[0x00] = { 0, 0, 0, 0 }; // null descriptor
    entries[0x01] = { 0, 0xfffff, Access_Present | Access_Ring0 | Access_Code_Segment | Access_Code_Readable, Flags_32Bit | Flags_Granularity_4K }; // kernel code segment
    entries[0x02] = { 0, 0xfffff, Access_Present | Access_Ring0 | Access_Data_Segment | Access_Data_Writeable, Flags_32Bit | Flags_Granularity_4K }; // kernel data segment
    entries[0x03] = { 0, 0xfffff, Access_Present | Access_Ring3 | Access_Code_Segment | Access_Code_Readable, Flags_32Bit | Flags_Granularity_4K }; // user code segment
    entries[0x04] = { 0, 0xfffff, Access_Present | Access_Ring3 | Access_Data_Segment | Access_Data_Writeable, Flags_32Bit | Flags_Granularity_4K }; // user data segment
    entries[0x05] = { reinterpret_cast<uptr>(&tss), sizeof(TSS) - 1, Access_Present | Access_Ring0 | Access_Task_Segment, Flags_32Bit | Flags_Granularity_1B }; // task state segment

    memset(&tss, 0, sizeof(TSS));
    tss.IOPB = sizeof(TSS);
    tss.SS0 = GDT_DATA_SEGMENT;
    tss.ESP0 = reinterpret_cast<uptr>(kernel_stack);

    LoadGDT(&descriptor, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}
