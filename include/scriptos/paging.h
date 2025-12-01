#pragma once

#include <scriptos/bitmap.h>
#include <scriptos/types.h>

constexpr u16 PAGE_SIZE = 0x1000;

constexpr u16 PAGE_PRESENT  = 0x001;
constexpr u16 PAGE_WRITABLE = 0x002;
constexpr u16 PAGE_USER     = 0x004;

namespace kernel
{
    class PageFrameAllocator
    {
    public:
        PageFrameAllocator(const kernel::Bitmap& bitmap);

        void LockPage(const void* address);
        void FreePage(const void* address);

        void LockPages(
            const void* address,
            usize count);
        void FreePages(
            const void* address,
            usize count);

        void* AllocatePhysicalPage();
        void* AllocatePhysicalPages(usize count);

    private:
        kernel::Bitmap m_Bitmap;
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

    using PageTable = PageTableEntry*;

    extern uptr HHDM_Offset;
    extern PageTable PML4_Base;

    bool IsPhysical(const void* maybe_physical_address);

    template<
        typename T = void*,
        typename S = void*>
    T PhysicalToVirtual(S physical_address)
    {
        return reinterpret_cast<T>(reinterpret_cast<uptr>(physical_address) + HHDM_Offset);
    }

    template<
        typename T = void*,
        typename S = void*>
    T VirtualToPhysical(S virtual_address)
    {
        return reinterpret_cast<T>(GetMapping(virtual_address));
    }

    void InitializePaging(uptr hhdm_offset);

    void WalkTable(
        out_stream stream,
        PageTable table,
        uptr virtual_base = 0,
        u8 level          = 4);

    bool MapPage(
        const void* virtual_address,
        const void* physical_address,
        bool writable      = false,
        bool user          = false,
        bool write_through = false,
        bool cache_disable = false,
        bool accessed      = false);

    bool MapPages(
        const void* virtual_address,
        const void* physical_address,
        usize count,
        bool writable      = false,
        bool user          = false,
        bool write_through = false,
        bool cache_disable = false,
        bool accessed      = false);

    PageTable GetOrCreateNextPageLevel(
        PageTable table,
        usize index,
        bool create);

    void* GetPageMapping(const void* virtual_address);

    inline void FlushPage(const void* virtual_address)
    {
        asm volatile("invlpg (%0)" : : "r"(virtual_address) : "memory");
    }
}
