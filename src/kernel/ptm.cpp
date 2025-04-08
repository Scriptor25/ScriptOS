#include <scriptos/kernel/cr.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/assert.hpp>
#include <scriptos/std/debug.hpp>
#include <scriptos/std/memory.hpp>

struct page_index
{
    page_index(uptr virtual_address)
    {
        pdi = (virtual_address >> 22) & 0x3FF;
        pti = (virtual_address >> 12) & 0x3FF;
    }

    uptr pdi;
    uptr pti;
};

PageTableManager &PageTableManager::GetKernelInstance()
{
    static PageTableManager instance;
    return instance;
}

void PageTableManager::Initialize(PageDirectoryEntry *page_directory)
{
    m_PageDirectory = page_directory;
}

bool PageTableManager::MapPage(void *virtual_address, void *physical_address, bool user)
{
    page_index index(reinterpret_cast<uptr>(virtual_address));
    PageTableEntry *pt;

    auto &pde = m_PageDirectory[index.pdi];
    pde.UserSupervisor = user;
    if (!pde.Present)
    {
        pt = reinterpret_cast<PageTableEntry *>(PageFrameAllocator::GetKernelInstance().RequestEmptyPage());
        assert(pt && "out of memory");

        pde.Present = true;
        pde.ReadWrite = true;
        pde.Address_31_12 = reinterpret_cast<uptr>(pt) >> 12;
    }
    else
    {
        pt = reinterpret_cast<PageTableEntry *>(pde.Address_31_12 << 12);
    }

    auto &pte = pt[index.pti];

    if (pte.Present)
    {
        auto pa = reinterpret_cast<void *>(pte.Address_31_12 << 12);
        if (pa == physical_address && pte.UserSupervisor == user)
            return false;

        debug("[WARNING] remapping page at %p from %p to %p for %s (previously owned by %s) [ %u | %u ]",
              virtual_address,
              pa,
              physical_address,
              user ? "user" : "supervisor",
              pte.UserSupervisor ? "user" : "supervisor",
              index.pdi,
              index.pti);
    }

    pte.UserSupervisor = user;
    pte.Present = true;
    pte.ReadWrite = true;
    pte.Address_31_12 = reinterpret_cast<uptr>(physical_address) >> 12;

    InvalidatePage(virtual_address);

    return false;
}

bool PageTableManager::MapPages(void *virtual_address, void *physical_address, usize count, bool user)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        if (MapPage(reinterpret_cast<void *>(reinterpret_cast<uptr>(virtual_address) + i), reinterpret_cast<void *>(reinterpret_cast<uptr>(physical_address) + i), user))
            return true;
    return false;
}

void PageTableManager::SetupPaging()
{
    CR::CR3::W(reinterpret_cast<uptr>(m_PageDirectory));
    auto cr0 = CR::CR0::R();
    cr0.PE = true;
    cr0.PG = true;
    CR::CR0::W(cr0);
}

bool PageTableManager::IsMapped(void *virtual_address) const
{
    page_index index(reinterpret_cast<uptr>(virtual_address));

    auto &pde = m_PageDirectory[index.pdi];
    if (!pde.Present)
        return false;

    auto &pte = reinterpret_cast<PageTableEntry *>(pde.Address_31_12 << 12)[index.pti];
    if (!pte.Present)
        return false;

    return true;
}

void InvalidatePage(void *address)
{
    asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}
