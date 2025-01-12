#include <scriptos/info.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/paging.hpp>
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
    auto memory_size = mmap.Size();
    m_FreeMemory = memory_size;
    auto bitmap_size = ceil_div(memory_size, PAGE_SIZE * 8);

    InitBitmap(bitmap_size, (u8 *)KERNEL_END);

    ReservePages((void *)0x00000000, ceil_div(memory_size, PAGE_SIZE));
    for (auto &entry : mmap)
        if (entry.type == MULTIBOOT_MEMORY_AVAILABLE && !entry.base_addr_hi && !entry.length_hi)
            UnreservePages((void *)entry.base_addr_lo, ceil_div(entry.length_lo, PAGE_SIZE));
    ReservePages((void *)0x00000000, 0x200);

    LockPages(KERNEL_END, ceil_div(bitmap_size, PAGE_SIZE));
}

void PageFrameAllocator::FreePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (!m_PageMap[index] || !m_PageMap.Clr(index))
        return;

    m_FreeMemory += PAGE_SIZE;
    m_UsedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::FreePages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        FreePage((void *)((uptr)address + i));
}

void PageFrameAllocator::LockPage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (m_PageMap[index] || !m_PageMap.Set(index))
        return;

    m_FreeMemory -= PAGE_SIZE;
    m_UsedMemory += PAGE_SIZE;
}

void PageFrameAllocator::LockPages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        LockPage((void *)((uptr)address + i));
}

void *PageFrameAllocator::RequestPage()
{
    auto map_count = m_PageMap.Size() * 8;
    for (usize i = 0; i < map_count; ++i)
        if (!m_PageMap[i])
        {
            auto address = (void *)(i * PAGE_SIZE);
            LockPage(address);
            return address;
        }
    return nullptr;
}

void *PageFrameAllocator::RequestEmptyPage()
{
    auto address = RequestPage();
    memset(address, 0, PAGE_SIZE);
    return address;
}

void *PageFrameAllocator::RequestPages(usize count)
{
    auto map_count = m_PageMap.Size() * 8;
    for (usize oi = 0; oi < map_count - count; ++oi)
        if (!m_PageMap[oi])
        {
            usize ii;
            for (ii = 0; (ii < count) && (oi + ii < map_count); ++ii)
                if (m_PageMap[oi + ii])
                    break;
            if (ii < count)
                continue;

            auto address = (void *)(oi * PAGE_SIZE);
            LockPages(address, count);
            return address;
        }

    return nullptr;
}

const Bitmap &PageFrameAllocator::PageMap() const { return m_PageMap; }

usize PageFrameAllocator::FreeMemory() const { return m_FreeMemory; }

usize PageFrameAllocator::UsedMemory() const { return m_UsedMemory; }

usize PageFrameAllocator::ReservedMemory() const { return m_ReservedMemory; }

void PageFrameAllocator::InitBitmap(usize size, u8 *buffer)
{
    m_PageMap.Init(size, buffer);
    memset(buffer, 0, size);
}

void PageFrameAllocator::ReservePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (m_PageMap[index] || !m_PageMap.Set(index))
        return;

    m_FreeMemory -= PAGE_SIZE;
    m_ReservedMemory += PAGE_SIZE;
}

void PageFrameAllocator::ReservePages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        ReservePage((void *)((uptr)address + i));
}

void PageFrameAllocator::UnreservePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (!m_PageMap[index] || !m_PageMap.Clr(index))
        return;

    m_FreeMemory += PAGE_SIZE;
    m_ReservedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::UnreservePages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        UnreservePage((void *)((uptr)address + i));
}
