#pragma once

#include <scriptos/bitmap.hpp>
#include <scriptos/mmap.hpp>

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

    usize m_FreeMemory = 0;
    usize m_UsedMemory = 0;
    usize m_ReservedMemory = 0;
};
