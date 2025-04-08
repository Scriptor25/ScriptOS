#include <scriptos/kernel/cr.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/memory.hpp>

struct page_index
{
    page_index(uptr virtual_address)
    {
        pdi = (virtual_address >> 22);
        pti = (virtual_address >> 12) & 0x3ff;
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

void PageTableManager::MapPage(void *virtual_address, void *physical_address, bool user)
{
    page_index index(reinterpret_cast<uptr>(virtual_address));
    PageTableEntry *pt;

    auto &pde = m_PageDirectory[index.pdi];
    pde.UserSupervisor = user;
    if (!pde.Present)
    {
        pt = reinterpret_cast<PageTableEntry *>(PageFrameAllocator::GetKernelInstance().RequestEmptyPage());

        pde.Present = true;
        pde.ReadWrite = true;
        pde.Address_31_12 = reinterpret_cast<uptr>(pt) >> 12;
    }
    else
    {
        pt = reinterpret_cast<PageTableEntry *>(pde.Address_31_12 << 12);
    }

    auto &pte = pt[index.pti];
    pte.UserSupervisor = user;
    pte.Present = true;
    pte.ReadWrite = true;
    pte.Address_31_12 = reinterpret_cast<uptr>(physical_address) >> 12;

    InvalidatePage(virtual_address);
}

void PageTableManager::MapPages(void *virtual_address, void *physical_address, usize count, bool user)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        MapPage(reinterpret_cast<void *>(reinterpret_cast<uptr>(virtual_address) + i), reinterpret_cast<void *>(reinterpret_cast<uptr>(physical_address) + i), user);
}

void PageTableManager::SetupPaging()
{
    CR::CR3::W(reinterpret_cast<uptr>(m_PageDirectory));
    auto cr0 = CR::CR0::R();
    cr0.PE = true;
    cr0.PG = true;
    CR::CR0::W(cr0);
}

void InvalidatePage(void *address)
{
    asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}
