#include <scriptos/paging.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>

#define NUM_ENTRIES 0x400

PageDirectoryEntry PageDirectory[NUM_ENTRIES] alignas(PAGE_SIZE);
PageTableEntry PageTable[NUM_ENTRIES] alignas(PAGE_SIZE);

PageIndex::PageIndex(uptr virtual_address)
{
    PDI = (virtual_address >> 22);
    PTI = (virtual_address >> 12) & 0x3ff;
}

void PageIndex::Print() const
{
    printf("%u - %u\n", (u32)PTI, (u32)PDI);
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
        pt = (PageTableEntry *)PageFrameAllocator::Get().RequestPage();
        memset(pt, 0, PAGE_SIZE);

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
    for (usize i = 0; i < count; ++i)
        MapPage((u8 *)virtual_address + (PAGE_SIZE * i), (u8 *)physical_address + (PAGE_SIZE * i));
}

void Paging_Setup()
{
    for (u32 i = 0; i < NUM_ENTRIES; i++)
        PageTable[i].Raw = (i * PAGE_SIZE) | 0x3;

    PageDirectory[0].Raw = ((uptr)PageTable) | 0x3;

    for (u32 i = 1; i < NUM_ENTRIES; i++)
        PageDirectory[i].Raw = 0;

    asm volatile("mov %0, %%cr3" : : "r"(PageDirectory));

    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000001;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}
