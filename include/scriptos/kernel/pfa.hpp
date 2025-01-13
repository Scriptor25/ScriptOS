#pragma once

#include <scriptos/kernel/bitmap.hpp>
#include <scriptos/kernel/mmap.hpp>

class PageFrameAllocator
{
public:
    static PageFrameAllocator &GetInstance();

    void Init(const MemoryMap &mmap);

    void FreePage(void *address);
    void FreePages(void *address, usize count);
    void LockPage(void *address);
    void LockPages(void *address, usize count);

    void *RequestPage();
    void *RequestEmptyPage();

    const Bitmap &PageMap() const;

    usize FreeMemory() const;
    usize UsedMemory() const;
    usize ReservedMemory() const;

private:
    void InitBitmap(usize bitmap_size, u8 *buffer);

    void ReservePage(void *address);
    void ReservePages(void *address, usize count);
    void UnreservePage(void *address);
    void UnreservePages(void *address, usize count);

    Bitmap m_PageMap;

    usize m_FreeMemory;
    usize m_UsedMemory;
    usize m_ReservedMemory;
};
