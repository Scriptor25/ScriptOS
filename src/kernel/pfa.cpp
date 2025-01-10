#include <scriptos/info.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>
#include <scriptos/util.hpp>

PageFrameAllocator PageFrameAllocator::INSTANCE;

PageFrameAllocator &PageFrameAllocator::Get()
{
    return INSTANCE;
}

void PageFrameAllocator::Init(const MemoryMap &mmap)
{
    auto memory_size = Memory_GetSize(mmap);
    m_FreeMemory = memory_size;
    auto bitmap_size = ceil_div(memory_size, PAGE_SIZE * 8);

    InitBitmap(bitmap_size, (u8 *)KERNEL_END);

    for (auto &entry : mmap)
        if (entry.type != MULTIBOOT_MEMORY_AVAILABLE || entry.base_addr == 0)
        {
            auto page_count = ceil_div(entry.length, PAGE_SIZE);
            ReservePages((void *)entry.base_addr, page_count);
        }

    LockPages((u8 *)KERNEL_END, ceil_div(bitmap_size, PAGE_SIZE));
}

void PageFrameAllocator::FreePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (!m_PageMap[index])
        return;

    m_PageMap.Set(index, false);
    m_FreeMemory += PAGE_SIZE;
    m_UsedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::FreePages(void *address, u32 count)
{
    for (u32 off = 0; off < count; ++off)
        FreePage((void *)((uptr)address + off * PAGE_SIZE));
}

void PageFrameAllocator::LockPage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (m_PageMap[index])
        return;

    m_PageMap.Set(index, true);
    m_FreeMemory -= PAGE_SIZE;
    m_UsedMemory += PAGE_SIZE;
}

void PageFrameAllocator::LockPages(void *address, u32 count)
{
    for (u32 off = 0; off < count; ++off)
        LockPage((void *)((uptr)address + off * PAGE_SIZE));
}

void *PageFrameAllocator::RequestPage()
{
    auto size = m_PageMap.GetSize();
    for (u32 index = 0; index < size; ++index)
        if (!m_PageMap[index])
        {
            auto address = (void *)(index * PAGE_SIZE);
            LockPage(address);
            return address;
        }
    return nullptr;
}

const Bitmap &PageFrameAllocator::GetPageMap() const { return m_PageMap; }

u32 PageFrameAllocator::GetFree() const { return m_FreeMemory; }

u32 PageFrameAllocator::GetUsed() const { return m_UsedMemory; }

u32 PageFrameAllocator::GetReserved() const { return m_ReservedMemory; }

void PageFrameAllocator::InitBitmap(u32 size, u8 *buffer)
{
    m_PageMap.Init(size, buffer);
    memset(buffer, 0, size);
}

void PageFrameAllocator::ReservePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (m_PageMap[index])
        return;

    m_PageMap.Set(index, true);
    m_FreeMemory -= PAGE_SIZE;
    m_ReservedMemory += PAGE_SIZE;
}

void PageFrameAllocator::ReservePages(void *address, u32 count)
{
    for (u32 off = 0; off < count; ++off)
        ReservePage((void *)((uptr)address + off * PAGE_SIZE));
}

void PageFrameAllocator::UnreservePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (!m_PageMap[index])
        return;

    m_PageMap.Set(index, false);
    m_FreeMemory += PAGE_SIZE;
    m_ReservedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::UnreservePages(void *address, u32 count)
{
    for (u32 off = 0; off < count; ++off)
        UnreservePage((void *)((uptr)address + off * PAGE_SIZE));
}
