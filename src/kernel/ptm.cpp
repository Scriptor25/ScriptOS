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

void PageTableManager::Init(PageDirectoryEntry *page_directory)
{
    m_PageDirectory = page_directory;
}

void PageTableManager::MapPage(void *virtual_address, void *physical_address)
{
    page_index index((uptr)virtual_address);
    PageTableEntry *pt;

    auto &pde = m_PageDirectory[index.pdi];
    if (!pde.Present)
    {
        pt = (PageTableEntry *)PageFrameAllocator::GetInstance().RequestEmptyPage();

        pde.Present = true;
        pde.ReadWrite = true;
        pde.Address_31_12 = (uptr)pt >> 12;
    }
    else
    {
        pt = (PageTableEntry *)(pde.Address_31_12 << 12);
    }

    auto &pte = pt[index.pti];
    pte.Present = true;
    pte.ReadWrite = true;
    pte.Address_31_12 = (uptr)physical_address >> 12;

    invlpg(virtual_address);
}

void PageTableManager::MapPages(void *virtual_address, void *physical_address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        MapPage((void *)((uptr)virtual_address + i), (void *)((uptr)physical_address + i));
}

void PageTableManager::SetupPaging()
{
    asm volatile("mov %0, %%cr3" : : "r"(m_PageDirectory));

    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000001;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}
