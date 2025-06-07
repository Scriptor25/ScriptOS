#pragma once

#include "scriptos/types.h"

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

    union PageTableEntry
    {
        struct
        {
            u8 Present        : 1;
            u8 ReadWrite      : 1;
            u8 UserSupervisor : 1;
            u8 WriteThrough   : 1;
            u8 CacheDisable   : 1;
            u8 Accessed       : 1;
            u16 AvailableLo   : 1;
            u8 PageSize       : 1;
            u16 AvailableMi   : 4;
            u64 Address       : 36;
            u8                : 4;
            u16 AvailableHi   : 11;
            u8 ExecuteDisable : 1;
        } __attribute__((packed));

        u64 Value;
    };

    typedef PageTableEntry* PageTable;

    extern PageTable PML4_Base;

    bool IsPhysical(void* maybe_physical_address);
    void* PhysicalToVirtual(void* physical_address, uptr offset);

    void Initialize();

    void WalkTable(out_stream stream, PageTable table, uptr offset, uptr virtual_base = 0, u8 level = 4);

    bool MapPage(PageFrameAllocator& allocator, void* virtual_address, void* physical_address, uptr virtual_offset, bool present = true, bool read_write = false, bool user_supervisor = false, bool write_through = false, bool cache_disable = false, bool accessed = false);
    bool MapPages(PageFrameAllocator& allocator, void* virtual_address, void* physical_address, usize count, uptr virtual_offset, bool present = true, bool read_write = false, bool user_supervisor = false, bool write_through = false, bool cache_disable = false, bool accessed = false);
    PageTable GetOrCreateNextLevel(PageFrameAllocator& allocator, PageTable table, uptr virtual_offset, usize index, bool create = true);

    void FlushPage(void* virtual_address);
}
