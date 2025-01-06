#include <scriptos/info.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>

void PageFrameAllocator::Init(const MemoryMap &mmap)
{
    u64 largest_addr = 0;
    u64 largest_length = 0;

    for (auto &entry : mmap)
    {
        if (entry.type != MULTIBOOT_MEMORY_AVAILABLE)
            continue;
        if (entry.length < largest_length)
            continue;

        largest_addr = entry.base_addr;
        largest_length = entry.length;
    }

    auto memory_size = Memory_GetSize(mmap);
    m_FreeMemory = memory_size;
    u64 bitmap_size = memory_size / (PAGE_SIZE * 8) + 1;

    printf("largest addr = %p, length = %uKB\n", (void *)largest_addr, (u32)(largest_length / 1024));
    printf("bitmap size = %uB\n", (u32)(bitmap_size));
    printf("kernel start = %p, end = %p\n", KERNEL_START, KERNEL_END);

    InitBitmap(bitmap_size, (u8 *)KERNEL_END);
    ReservePages(KERNEL_START, ((u64)KERNEL_END - (u64)KERNEL_START) / PAGE_SIZE + 1);
    LockPages(KERNEL_END, bitmap_size / PAGE_SIZE + 1);

    for (auto &entry : mmap)
        if (entry.type != MULTIBOOT_MEMORY_AVAILABLE)
        {
            auto rem = entry.length % PAGE_SIZE;
            auto page_count = entry.length / PAGE_SIZE;
            if (rem)
                page_count++;
            // ReservePages((void *)entry.base_addr, page_count);
            printf("reserve pages address = %p, count = %u\n", (void *)entry.base_addr, page_count);
        }
}

void PageFrameAllocator::FreePage(void *address)
{
    u64 index = (u64)address / PAGE_SIZE;
    if (!m_PageMap[index])
        return;

    m_PageMap.Put(index, false);
    m_FreeMemory += PAGE_SIZE;
    m_UsedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::FreePages(void *address, u64 count)
{
    for (u64 off = 0; off < count; ++off)
        FreePage((void *)((u64)address + off * PAGE_SIZE));
}

void PageFrameAllocator::LockPage(void *address)
{
    u64 index = (u64)address / PAGE_SIZE;
    if (m_PageMap[index])
        return;

    m_PageMap.Put(index, true);
    m_FreeMemory -= PAGE_SIZE;
    m_UsedMemory += PAGE_SIZE;
}

void PageFrameAllocator::LockPages(void *address, u64 count)
{
    for (u64 off = 0; off < count; ++off)
        LockPage((void *)((u64)address + off * PAGE_SIZE));
}

u64 PageFrameAllocator::GetFree() const { return m_FreeMemory; }

u64 PageFrameAllocator::GetUsed() const { return m_UsedMemory; }

u64 PageFrameAllocator::GetReserved() const { return m_ReservedMemory; }

void PageFrameAllocator::InitBitmap(u64 size, u8 *buffer)
{
    m_PageMap.Init(size, buffer);
    memset(buffer, 0, size);
}

void PageFrameAllocator::ReservePage(void *address)
{
    u64 index = (u64)address / PAGE_SIZE;
    if (!m_PageMap[index])
        return;

    m_PageMap.Put(index, false);
    m_FreeMemory -= PAGE_SIZE;
    m_ReservedMemory += PAGE_SIZE;
}

void PageFrameAllocator::ReservePages(void *address, u64 count)
{
    for (u64 off = 0; off < count; ++off)
        ReservePage((void *)((u64)address + off * PAGE_SIZE));
}

void PageFrameAllocator::UnreservePage(void *address)
{
    u64 index = (u64)address / PAGE_SIZE;
    if (m_PageMap[index])
        return;

    m_PageMap.Put(index, true);
    m_FreeMemory += PAGE_SIZE;
    m_ReservedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::UnreservePages(void *address, u64 count)
{
    for (u64 off = 0; off < count; ++off)
        UnreservePage((void *)((u64)address + off * PAGE_SIZE));
}
