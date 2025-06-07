#pragma once

#include <scriptos/bitmap.h>

#define PAGE_SIZE 0x1000

#define PAGE_PRESENT  0x001
#define PAGE_WRITABLE 0x002
#define PAGE_USER     0x004

namespace paging
{
    class PageFrameAllocator
    {
    public:
        PageFrameAllocator() = default;
        PageFrameAllocator(const Bitmap& bitmap);

        void LockPage(void* address);
        void FreePage(void* address);

        void LockPages(void* address, usize count);
        void FreePages(void* address, usize count);

        void* AllocatePhysicalPage();
        void* AllocatePhysicalPages(usize count);

    private:
        Bitmap m_Bitmap;
    };

    typedef u64 PageTableEntry;

    typedef PageTableEntry* PageTable;

    extern PageTable PML4_Base;

    void Initialize();
    bool MapPage(PageFrameAllocator& allocator, void* virtual_address, void* physical_address, u64 flags);
    void FlushPage(void* virtual_address);
}
