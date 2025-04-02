#pragma once

#include <scriptos/kernel/bitmap.hpp>
#include <scriptos/kernel/mmap.hpp>

/**
 * Page Frame Allocator
 */
class PageFrameAllocator
{
public:
    /**
     * Get the kernel managed instance of the page frame allocator.
     */
    static PageFrameAllocator &GetInstance();

    /**
     * Initialize this page frame allocator with a memory map.
     */
    void Init(const MemoryMap &mmap);

    /**
     * Mark a single addressed page as free.
     */
    void FreePage(void *address);
    /**
     * Mark multiple pages starting at the address as free.
     */
    void FreePages(void *address, usize count);
    /**
     * Mark a single addressed page as locked.
     */
    void LockPage(void *address);
    /**
     * Mark multiple pages starting at the address as locked.
     */
    void LockPages(void *address, usize count);

    /**
     * Request a single free page.
     */
    void *RequestPage();
    /**
     * Request a single free page with all zeros.
     */
    void *RequestEmptyPage();

    /**
     * Get a immutable view of the underlying bitmap.
     */
    const Bitmap &PageMap() const;

    /**
     * Get the amount of free memory (in byte).
     */
    usize FreeMemory() const;
    /**
     * Get the amount of used memory (in byte).
     */
    usize UsedMemory() const;
    /**
     * Get the amount of reserved memory (in byte).
     */
    usize ReservedMemory() const;

private:
    /**
     * Initialize the underlying page bitmap.
     */
    void InitBitmap(usize bitmap_size, u8 *buffer);

    /**
     * Mark a single addressed page as reserved.
     */
    void ReservePage(void *address);
    /**
     * Mark multiple pages starting at the address as reserved.
     */
    void ReservePages(void *address, usize count);
    /**
     * Mark a single addressed page as unreserved.
     */
    void UnreservePage(void *address);
    /**
     * Mark multiple pages starting at the address as unreserved.
     */
    void UnreservePages(void *address, usize count);

    Bitmap m_PageMap;

    usize m_FreeMemory;
    usize m_UsedMemory;
    usize m_ReservedMemory;
};
