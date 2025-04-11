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

bool PageTableManager::MapPage(const void *virtual_address, const void *physical_address, bool user)
{
    if (reinterpret_cast<uptr>(virtual_address) & (PAGE_SIZE - 1))
    {
        debug("cannot map non-aligned page at %p to %p for %s", virtual_address, physical_address, user ? "user" : "supervisor");
        return false;
    }
    if (reinterpret_cast<uptr>(physical_address) & (PAGE_SIZE - 1))
    {
        debug("cannot map non-aligned page at %p to %p for %s", virtual_address, physical_address, user ? "user" : "supervisor");
        return false;
    }

    page_index index(reinterpret_cast<uptr>(virtual_address));
    PageTableEntry *pt;

    auto &pde = m_PageDirectory[index.pdi];
    if (pde.Present)
    {
        pt = reinterpret_cast<PageTableEntry *>(pde.Address_31_12 << 12);

        auto us = pde.UserSupervisor;

        if (us != user)
        {
            debug("remapping page directory entry at %p from %s to %s access",
                  virtual_address,
                  us ? "user" : "supervisor",
                  user ? "user" : "supervisor");
            pde.UserSupervisor = user;
        }
    }
    else
    {
        auto page = PageFrameAllocator::GetKernelInstance().RequestEmptyPage();
        assert(page && "out of memory");

        pt = reinterpret_cast<PageTableEntry *>(page);

        pde.Present = true;
        pde.ReadWrite = true;
        pde.UserSupervisor = user;
        pde.Address_31_12 = reinterpret_cast<uptr>(pt) >> 12;
    }

    auto &pte = pt[index.pti];
    if (pte.Present)
    {
        auto pa = reinterpret_cast<void *>(pte.Address_31_12 << 12);
        auto us = pte.UserSupervisor;

        if ((pa == physical_address) && (us == user))
            return true;

        debug("remapping page at %p from %p to %p for %s (previously owned by %s) [ %u | %u ]",
              virtual_address,
              pa,
              physical_address,
              user ? "user" : "supervisor",
              us ? "user" : "supervisor",
              index.pdi,
              index.pti);

        pte.UserSupervisor = user;
        pte.Address_31_12 = reinterpret_cast<uptr>(physical_address) >> 12;
    }
    else
    {
        pte.Present = true;
        pte.ReadWrite = true;
        pte.UserSupervisor = user;
        pte.Address_31_12 = reinterpret_cast<uptr>(physical_address) >> 12;
    }

    InvalidatePage(virtual_address);
    return true;
}

bool PageTableManager::MapPages(const void *virtual_address, const void *physical_address, usize count, bool user)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        if (!MapPage(reinterpret_cast<void *>(reinterpret_cast<uptr>(virtual_address) + i), reinterpret_cast<void *>(reinterpret_cast<uptr>(physical_address) + i), user))
            return false;
    return true;
}

void PageTableManager::SetupPaging()
{
    CR::CR3::W(reinterpret_cast<uptr>(m_PageDirectory));
    auto cr0 = CR::CR0::R();
    cr0.PE = true;
    cr0.PG = true;
    CR::CR0::W(cr0);
}

bool PageTableManager::IsMapped(const void *virtual_address) const
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

void InvalidatePage(const void *address)
{
    asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}
