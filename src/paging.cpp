#include <scriptos/format.h>
#include <scriptos/paging.h>
#include <scriptos/types.h>

uptr paging::HHDM_Offset;
paging::PageTable paging::PML4_Base;

bool paging::IsPhysical(void* maybe_physical_address)
{
    return !((reinterpret_cast<uptr>(maybe_physical_address) >> 48) & 0xffff);
}

void* paging::PhysicalToVirtual(void* physical_address)
{
    return reinterpret_cast<void*>(reinterpret_cast<uptr>(physical_address) + HHDM_Offset);
}

void paging::Initialize(uptr hhdm_offset)
{
    HHDM_Offset = hhdm_offset;

    u64 reg;
    asm volatile("mov %%cr3, %0" : "=r"(reg));
    PML4_Base = reinterpret_cast<PageTable>(reg & ~((1lu << 12) - 1));
}

void paging::WalkTable(out_stream stream, PageTable table, uptr virtual_base, u8 level)
{
    for (unsigned i = 0; i < 512; ++i)
    {
        auto entry = table[i];
        if (!entry.Present)
            continue;

        auto physical_address = reinterpret_cast<void*>(entry.Address << 12);
        auto virtual_address = virtual_base + (static_cast<u64>(i) << (12 + (level - 1) * 9));

        if (level <= 1)
        {
            Print(stream, "at %016X -> %016X\r\n", virtual_address, physical_address);
            continue;
        }

        PageTable next_table;
        if (IsPhysical(physical_address))
            next_table = reinterpret_cast<PageTable>(PhysicalToVirtual(physical_address));
        else
            next_table = reinterpret_cast<PageTable>(physical_address);

        WalkTable(stream, next_table, virtual_address, level - 1);
    }
}

bool paging::MapPage(PageFrameAllocator& allocator, void* virtual_address, void* physical_address, bool present, bool read_write, bool user_supervisor, bool write_through, bool cache_disable, bool accessed)
{
    // lvl4 = [47:39]
    usize lvl4 = (reinterpret_cast<uptr>(virtual_address) >> 39) & 0x1FF;
    // lvl3 = [38:30]
    usize lvl3 = (reinterpret_cast<uptr>(virtual_address) >> 30) & 0x1FF;
    // lvl2 = [29:21]
    usize lvl2 = (reinterpret_cast<uptr>(virtual_address) >> 21) & 0x1FF;
    // lvl1 = [20:12]
    usize lvl1 = (reinterpret_cast<uptr>(virtual_address) >> 12) & 0x1FF;
    // lvl0 = [11:00] --> not needed

    auto pdpt = GetOrCreateNextLevel(allocator, PML4_Base, lvl4);
    if (!pdpt)
        return false;

    auto pd = GetOrCreateNextLevel(allocator, pdpt, lvl3);
    if (!pd)
        return false;

    auto pt = GetOrCreateNextLevel(allocator, pd, lvl2);
    if (!pt)
        return false;

    if (IsPhysical(pt))
        pt = reinterpret_cast<PageTable>(PhysicalToVirtual(pt));

    pt[lvl1].Value = 0;

    pt[lvl1].Present = present;
    pt[lvl1].ReadWrite = read_write;
    pt[lvl1].UserSupervisor = user_supervisor;
    pt[lvl1].WriteThrough = write_through;
    pt[lvl1].CacheDisable = cache_disable;
    pt[lvl1].Accessed = accessed;
    pt[lvl1].Address = reinterpret_cast<uptr>(physical_address) >> 12;

    FlushPage(virtual_address);

    return true;
}

bool paging::MapPages(PageFrameAllocator& allocator, void* virtual_address, void* physical_address, usize count, bool present, bool read_write, bool user_supervisor, bool write_through, bool cache_disable, bool accessed)
{
    for (usize i = 0; i < count; ++i)
    {
        auto pi = i * PAGE_SIZE;
        if (!MapPage(allocator, reinterpret_cast<void*>(reinterpret_cast<uptr>(virtual_address) + pi), reinterpret_cast<void*>(reinterpret_cast<uptr>(physical_address) + pi), present, read_write, user_supervisor, write_through, cache_disable, accessed))
            return false;
    }
    return true;
}

paging::PageTable paging::GetOrCreateNextLevel(PageFrameAllocator& allocator, PageTable table, usize index, bool create)
{
    if (IsPhysical(table))
        table = reinterpret_cast<PageTable>(PhysicalToVirtual(table));

    if (table[index].Present)
        return reinterpret_cast<PageTable>(table[index].Address << 12);

    if (!create)
        return nullptr;

    auto physical_address = allocator.AllocatePhysicalPage();
    if (!physical_address)
        return nullptr;

    auto next_table = reinterpret_cast<PageTable>(PhysicalToVirtual(physical_address));

    for (unsigned i = 0; i < 512; ++i)
        next_table[i].Value = 0;

    table[index].Value = 0;

    table[index].Present = true;
    table[index].ReadWrite = true;
    table[index].Address = reinterpret_cast<uptr>(physical_address) >> 12;

    return next_table;
}

void paging::FlushPage(void* virtual_address)
{
    asm volatile("invlpg (%0)" : : "r"(virtual_address) : "memory");
}
