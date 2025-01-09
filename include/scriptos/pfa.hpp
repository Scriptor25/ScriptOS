#pragma once

#include <scriptos/bitmap.hpp>
#include <scriptos/memory.hpp>

#define PAGE_SIZE 0x1000

class PageFrameAllocator
{
public:
    void Init(const MemoryMap &mmap);

    void FreePage(void *address);
    void FreePages(void *address, u64 count);
    void LockPage(void *address);
    void LockPages(void *address, u64 count);

    void *RequestPage();

    const Bitmap &GetPageMap() const;

    u64 GetFree() const;
    u64 GetUsed() const;
    u64 GetReserved() const;

private:
    void InitBitmap(u64 bitmap_size, u8 *buffer);

    void ReservePage(void *address);
    void ReservePages(void *address, u64 count);
    void UnreservePage(void *address);
    void UnreservePages(void *address, u64 count);

    Bitmap m_PageMap;

    u64 m_FreeMemory = 0;
    u64 m_UsedMemory = 0;
    u64 m_ReservedMemory = 0;
};
