#include <scriptos/memory.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/ptm.hpp>

PageIndex::PageIndex(uptr virtual_address)
{
    PDI = (virtual_address >> 22);
    PTI = (virtual_address >> 12) & 0x3ff;
}

PageTableManager::PageTableManager(PageDirectoryEntry *pd)
    : m_PD(pd)
{
}

void PageTableManager::MapPage(void *virtual_address, void *physical_address)
{
    PageIndex index((uptr)virtual_address);
    PageTableEntry *pt;

    auto &pde = m_PD[index.PDI];
    if (!pde.Present)
    {
        pt = (PageTableEntry *)PageFrameAllocator::Get().RequestEmptyPage();

        pde.Present = true;
        pde.ReadWrite = true;
        pde.Address_31_12 = (uptr)pt >> 12;
    }
    else
    {
        pt = (PageTableEntry *)(pde.Address_31_12 << 12);
    }

    auto &pte = pt[index.PTI];
    pte.Present = true;
    pte.ReadWrite = true;
    pte.Address_31_12 = (uptr)physical_address >> 12;

    asm volatile(
        "invlpg (%0);"
        :
        : "r"(virtual_address)
        : "memory");
}

void PageTableManager::MapPages(void *virtual_address, void *physical_address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        MapPage((u8 *)virtual_address + i, (u8 *)physical_address + i);
}

void PageTableManager::SetupPaging()
{
    asm volatile("mov %0, %%cr3" : : "r"(m_PD));

    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000001;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}
