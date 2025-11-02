#include <scriptos/kernel.h>
#include <scriptos/paging.h>
#include <scriptos/print.h>
#include <scriptos/types.h>

uptr paging::HHDM_Offset;
paging::PageTable paging::PML4_Base;

bool paging::IsPhysical(const void* maybe_physical_address)
{
    return !((reinterpret_cast<uptr>(maybe_physical_address) >> 48) & 0xffff);
}

void paging::Initialize(uptr hhdm_offset)
{
    HHDM_Offset = hhdm_offset;

    u64 reg;
    asm volatile("mov %%cr3, %0" : "=r"(reg));
    PML4_Base = reinterpret_cast<PageTable>(reg & ~((1lu << 12) - 1));
}

void paging::WalkTable(
    out_stream stream,
    PageTable table,
    uptr virtual_base,
    u8 level)
{
    for (unsigned i = 0; i < 512; ++i)
    {
        auto entry = table[i];
        if (!entry.Present)
        {
            continue;
        }

        auto physical_address = reinterpret_cast<void*>(entry.Address << 12);
        auto virtual_address = virtual_base + (static_cast<u64>(i) << (12 + (level - 1) * 9));

        if (level <= 1)
        {
            fkprintf(stream, "at %016X -> %016X\r\n", virtual_address, physical_address);
            continue;
        }

        PageTable next_table;
        if (IsPhysical(physical_address))
        {
            next_table = PhysicalToVirtual<PageTable>(physical_address);
        }
        else
        {
            next_table = reinterpret_cast<PageTable>(physical_address);
        }

        WalkTable(stream, next_table, virtual_address, level - 1);
    }
}

bool paging::MapPage(
    const void* virtual_address,
    const void* physical_address,
    bool writable,
    bool user,
    bool write_through,
    bool cache_disable,
    bool accessed)
{
    // lvl4 = [47:39]
    usize lvl4 = (reinterpret_cast<uptr>(virtual_address) >> 39) & 0x1FF;
    // lvl3 = [38:30]
    usize lvl3 = (reinterpret_cast<uptr>(virtual_address) >> 30) & 0x1FF;
    // lvl2 = [29:21]
    usize lvl2 = (reinterpret_cast<uptr>(virtual_address) >> 21) & 0x1FF;
    // lvl1 = [20:12]
    usize lvl1 = (reinterpret_cast<uptr>(virtual_address) >> 12) & 0x1FF;

    auto pdpt = GetOrCreateNextLevel(PML4_Base, lvl4, true);
    if (!pdpt)
    {
        return false;
    }

    auto pd = GetOrCreateNextLevel(pdpt, lvl3, true);
    if (!pd)
    {
        return false;
    }

    auto pt = GetOrCreateNextLevel(pd, lvl2, true);
    if (!pt)
    {
        return false;
    }

    if (IsPhysical(pt))
    {
        pt = PhysicalToVirtual<PageTable>(pt);
    }

    auto& pte = pt[lvl1];
    pte.Value = 0;

    pte.Present = 1;
    pte.ReadWrite = writable;
    pte.UserSupervisor = user;
    pte.WriteThrough = write_through;
    pte.CacheDisable = cache_disable;
    pte.Accessed = accessed;
    pte.Address = reinterpret_cast<uptr>(physical_address) >> 12;

    FlushPage(virtual_address);
    return true;
}

bool paging::MapPages(
    const void* virtual_address,
    const void* physical_address,
    usize count,
    bool writable,
    bool user,
    bool write_through,
    bool cache_disable,
    bool accessed)
{
    for (usize i = 0; i < count; ++i)
    {
        auto pi = i * PAGE_SIZE;
        if (!MapPage(reinterpret_cast<void*>(reinterpret_cast<uptr>(virtual_address) + pi), reinterpret_cast<void*>(reinterpret_cast<uptr>(physical_address) + pi), writable, user, write_through, cache_disable, accessed))
        {
            return false;
        }
    }
    return true;
}

paging::PageTable paging::GetOrCreateNextLevel(
    PageTable table,
    usize index,
    bool create)
{
    if (IsPhysical(table))
    {
        table = PhysicalToVirtual<PageTable>(table);
    }

    if (table[index].Present)
    {
        return reinterpret_cast<PageTable>(table[index].Address << 12);
    }

    if (!create)
    {
        return nullptr;
    }

    auto physical_address = kernel::Instance.Allocator->AllocatePhysicalPage();
    if (!physical_address)
    {
        return nullptr;
    }

    auto next_table = PhysicalToVirtual<PageTable>(physical_address);

    for (unsigned i = 0; i < 512; ++i)
    {
        next_table[i].Value = 0;
    }

    auto& entry = table[index];
    entry.Value = 0;

    entry.Present = true;
    entry.ReadWrite = true;
    entry.Address = reinterpret_cast<uptr>(physical_address) >> 12;

    return next_table;
}

void* paging::GetMapping(const void* virtual_address)
{
    // lvl4 = [47:39]
    usize lvl4 = (reinterpret_cast<uptr>(virtual_address) >> 39) & 0x1FF;
    // lvl3 = [38:30]
    usize lvl3 = (reinterpret_cast<uptr>(virtual_address) >> 30) & 0x1FF;
    // lvl2 = [29:21]
    usize lvl2 = (reinterpret_cast<uptr>(virtual_address) >> 21) & 0x1FF;
    // lvl1 = [20:12]
    usize lvl1 = (reinterpret_cast<uptr>(virtual_address) >> 12) & 0x1FF;

    auto pdpt = GetOrCreateNextLevel(PML4_Base, lvl4, false);
    if (!pdpt)
    {
        return nullptr;
    }

    auto pd = GetOrCreateNextLevel(pdpt, lvl3, false);
    if (!pd)
    {
        return nullptr;
    }

    auto pt = GetOrCreateNextLevel(pd, lvl2, false);
    if (!pt)
    {
        return nullptr;
    }

    if (IsPhysical(pt))
    {
        pt = PhysicalToVirtual<PageTable>(pt);
    }

    if (!pt[lvl1].Present)
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(pt[lvl1].Address << 12);
}
