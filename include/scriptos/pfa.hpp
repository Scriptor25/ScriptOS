#pragma once

#include <scriptos/bitmap.hpp>
#include <scriptos/memory.hpp>

#define PAGE_SIZE 0x1000

class PageFrameAllocator
{
public:
    static PageFrameAllocator &Get();

    void Init(const MemoryMap &mmap);

    void FreePage(void *address);
    void FreePages(void *address, u32 count);
    void LockPage(void *address);
    void LockPages(void *address, u32 count);

    void *RequestPage();

    const Bitmap &GetPageMap() const;

    u32 GetFree() const;
    u32 GetUsed() const;
    u32 GetReserved() const;

private:
    void InitBitmap(u32 bitmap_size, u8 *buffer);

    void ReservePage(void *address);
    void ReservePages(void *address, u32 count);
    void UnreservePage(void *address);
    void UnreservePages(void *address, u32 count);

    Bitmap m_PageMap;

    u32 m_FreeMemory = 0;
    u32 m_UsedMemory = 0;
    u32 m_ReservedMemory = 0;

    static PageFrameAllocator INSTANCE;
};
