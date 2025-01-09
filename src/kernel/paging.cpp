#include <scriptos/paging.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>

#define NUM_ENTRIES 0x400

static u32 PD[NUM_ENTRIES] alignas(PAGE_SIZE);
static u32 PT[NUM_ENTRIES] alignas(PAGE_SIZE);

PageIndex::PageIndex(u64 virtual_address)
{
    PD = (virtual_address >> 22);
    PT = (virtual_address >> 12) & 0x3ff;
}

void PageIndex::Print() const
{
    printf("%u - %u\n", (u32)PT, (u32)PD);
}

PageTableManager::PageTableManager(PageDirectoryEntry *pd)
    : m_PD(pd)
{
}

void PageTableManager::MapPage(void *virtual_address, void *physical_address)
{
    PageIndex index((u64)virtual_address);

    PageTableEntry *pt;

    auto &pde = m_PD[index.PD];
    if (!pde.Present)
    {
        pt = (PageTableEntry *)PageFrameAllocator::Get().RequestPage();
        memset(pt, 0, PAGE_SIZE);

        pde.Address_31_12 = (uptr)pt >> 12;
        pde.Present = true;
        pde.ReadWrite = true;
    }
    else
    {
        pt = (PageTableEntry *)(pde.Address_31_12 << 12);
    }

    

    auto& pte = pt[index.PT];
    if (!pte.Present)
    {

    }
}

void setup_paging()
{
    for (u32 i = 0; i < NUM_ENTRIES; i++)
        PT[i] = (i * PAGE_SIZE) | 0x3;

    PD[0] = ((u32)PT) | 0x3;

    for (u32 i = 1; i < NUM_ENTRIES; i++)
        PD[i] = 0;

    asm volatile("mov %0, %%cr3" : : "r"(PD));

    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000001;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}
