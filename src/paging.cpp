#include <scriptos/paging.h>

paging::PageTable paging::PML4_Base;

void paging::Initialize()
{
    u64 reg;
    asm volatile("mov %%cr3, %0" : "=r"(reg));
    PML4_Base = reinterpret_cast<PageTable>(reg & ~((1lu << 12) - 1));
}

static paging::PageTable get_or_create_next_level(paging::PageFrameAllocator& allocator, paging::PageTable table, usize index, bool create)
{
    if (table[index] & PAGE_PRESENT)
        return reinterpret_cast<paging::PageTable>(table[index] & 0x000F'FFFF'FFFF'F000);

    if (create)
    {
        auto sub_table = reinterpret_cast<paging::PageTable>(allocator.AllocatePhysicalPage());
        if (!sub_table)
            return nullptr;

        for (unsigned i = 0; i < 512; i++)
            sub_table[i] = 0;

        table[index] = reinterpret_cast<uptr>(sub_table) | PAGE_PRESENT | PAGE_WRITABLE;
        return sub_table;
    }

    return nullptr;
}

bool paging::MapPage(PageFrameAllocator& allocator, void* virtual_address, void* physical_address, u64 flags)
{
    usize pml4i = (reinterpret_cast<uptr>(virtual_address) >> 39) & 0x1FF;
    usize pdpti = (reinterpret_cast<uptr>(virtual_address) >> 30) & 0x1FF;
    usize pdi = (reinterpret_cast<uptr>(virtual_address) >> 21) & 0x1FF;
    usize pti = (reinterpret_cast<uptr>(virtual_address) >> 12) & 0x1FF;

    auto pdpt = get_or_create_next_level(allocator, PML4_Base, pml4i, true);
    if (!pdpt)
        return false;

    auto pd = get_or_create_next_level(allocator, pdpt, pdpti, true);
    if (!pd)
        return false;

    auto pt = get_or_create_next_level(allocator, pd, pdi, true);
    if (!pt)
        return false;

    pt[pti] = (reinterpret_cast<uptr>(physical_address) & 0x000F'FFFF'FFFF'F000) | (flags & 0xFFF) | PAGE_PRESENT;
    FlushPage(virtual_address);
    return true;
}

void paging::FlushPage(void* virtual_address)
{
    asm volatile("invlpg (%0)" : : "r"(virtual_address) : "memory");
}
