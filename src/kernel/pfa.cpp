#include <scriptos/kernel/info.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/kernel/paging.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/util.hpp>

PageFrameAllocator &PageFrameAllocator::GetKernelInstance()
{
    static PageFrameAllocator instance;
    return instance;
}

void PageFrameAllocator::Initialize(const MemoryMap &mmap)
{
    auto memory_size = mmap.Size();
    auto bitmap_size = ceil_div(memory_size, PAGE_SIZE * 8);

    m_FreeMemory = memory_size;
    m_UsedMemory = 0;
    m_ReservedMemory = 0;

    InitBitmap(bitmap_size, reinterpret_cast<u8 *>(KERNEL_END));

    ReservePages(reinterpret_cast<void *>(0x00000000), ceil_div(memory_size, PAGE_SIZE));
    for (auto &entry : mmap)
        if (entry.type == MULTIBOOT_MEMORY_AVAILABLE && !entry.base_addr_hi && !entry.length_hi)
            UnreservePages(reinterpret_cast<void *>(entry.base_addr_lo), ceil_div(entry.length_lo, PAGE_SIZE));
    ReservePages(reinterpret_cast<void *>(0x00000000), 0x200);

    LockPages(KERNEL_END, ceil_div(bitmap_size, PAGE_SIZE));
}

void PageFrameAllocator::FreePage(void *address)
{
    auto index = reinterpret_cast<uptr>(address) / PAGE_SIZE;
    if (!m_PageMap[index] || !m_PageMap.Clr(index))
        return;

    m_FreeMemory += PAGE_SIZE;
    m_UsedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::FreePages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        FreePage(reinterpret_cast<void *>(reinterpret_cast<uptr>(address) + i));
}

void PageFrameAllocator::LockPage(void *address)
{
    auto index = reinterpret_cast<uptr>(address) / PAGE_SIZE;
    if (m_PageMap[index] || !m_PageMap.Set(index))
        return;

    m_FreeMemory -= PAGE_SIZE;
    m_UsedMemory += PAGE_SIZE;
}

void PageFrameAllocator::LockPages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        LockPage(reinterpret_cast<void *>(reinterpret_cast<uptr>(address) + i));
}

void *PageFrameAllocator::RequestPage()
{
    auto map_count = m_PageMap.Size() * 8;
    for (usize i = 0; i < map_count; ++i)
        if (!m_PageMap[i])
        {
            auto address = reinterpret_cast<void *>(i * PAGE_SIZE);
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

const Bitmap &PageFrameAllocator::PageMap() const { return m_PageMap; }

usize PageFrameAllocator::FreeMemory() const { return m_FreeMemory; }

usize PageFrameAllocator::UsedMemory() const { return m_UsedMemory; }

usize PageFrameAllocator::ReservedMemory() const { return m_ReservedMemory; }

void PageFrameAllocator::InitBitmap(usize size, u8 *buffer)
{
    m_PageMap.Initialize(size, buffer);
    memset(buffer, 0, size);
}

void PageFrameAllocator::ReservePage(void *address)
{
    auto index = reinterpret_cast<uptr>(address) / PAGE_SIZE;
    if (m_PageMap[index] || !m_PageMap.Set(index))
        return;

    m_FreeMemory -= PAGE_SIZE;
    m_ReservedMemory += PAGE_SIZE;
}

void PageFrameAllocator::ReservePages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        ReservePage(reinterpret_cast<void *>(reinterpret_cast<uptr>(address) + i));
}

void PageFrameAllocator::UnreservePage(void *address)
{
    auto index = reinterpret_cast<uptr>(address) / PAGE_SIZE;
    if (!m_PageMap[index] || !m_PageMap.Clr(index))
        return;

    m_FreeMemory += PAGE_SIZE;
    m_ReservedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::UnreservePages(void *address, usize count)
{
    auto size = count * PAGE_SIZE;
    for (usize i = 0; i < size; i += PAGE_SIZE)
        UnreservePage(reinterpret_cast<void *>(reinterpret_cast<uptr>(address) + i));
}
