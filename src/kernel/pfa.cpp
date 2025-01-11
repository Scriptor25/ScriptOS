#include <scriptos/info.hpp>
#include <scriptos/memory.hpp>
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
    auto bitmap_size = ceil_div<usize>(memory_size, PAGE_SIZE * 8);

    InitBitmap(bitmap_size, (u8 *)KERNEL_END);

    ReservePages((void *)0x00000000, ceil_div<usize>(0x000003ff - 0x00000000, PAGE_SIZE)); // interrupt vector table
    ReservePages((void *)0x00000400, ceil_div<usize>(0x000004ff - 0x00000400, PAGE_SIZE)); // bios data area
    ReservePages((void *)0x00000500, ceil_div<usize>(0x000007ff - 0x00000500, PAGE_SIZE)); // grub data area
    // ReservePages((void *)0x00000800, ceil_div<usize>(0x0009efff - 0x00000800, PAGE_SIZE)); // crashes for some reason
    ReservePages((void *)0x0009f000, ceil_div<usize>(0x0009ffff - 0x0009f000, PAGE_SIZE)); // extended bios data area
    ReservePages((void *)0x000a0000, ceil_div<usize>(0x000fffff - 0x000a0000, PAGE_SIZE)); // video, vga bios, system bios and others

    for (auto &entry : mmap)
        if (entry.type != MULTIBOOT_MEMORY_AVAILABLE)
            ReservePages((void *)entry.base_addr, ceil_div<usize>(entry.length, PAGE_SIZE));

    LockPages(KERNEL_END, ceil_div<usize>(bitmap_size, PAGE_SIZE));
}

void PageFrameAllocator::FreePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (!m_PageMap[index])
        return;
    if (!m_PageMap.Set(index, false))
        return;

    m_FreeMemory += PAGE_SIZE;
    m_UsedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::FreePages(void *address, usize count)
{
    for (usize off = 0; off < count; ++off)
        FreePage((void *)((uptr)address + off * PAGE_SIZE));
}

void PageFrameAllocator::LockPage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (m_PageMap[index])
        return;
    if (!m_PageMap.Set(index, true))
        return;

    m_FreeMemory -= PAGE_SIZE;
    m_UsedMemory += PAGE_SIZE;
}

void PageFrameAllocator::LockPages(void *address, usize count)
{
    for (usize off = 0; off < count; ++off)
        LockPage((void *)((uptr)address + off * PAGE_SIZE));
}

void *PageFrameAllocator::RequestPage()
{
    auto size = m_PageMap.GetSize() * 8;
    for (usize index = 0; index < size; ++index)
        if (!m_PageMap[index])
        {
            auto address = (void *)(index * PAGE_SIZE);
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
    auto size = m_PageMap.GetSize() * 8;
    for (usize oi = 0; oi < size; ++oi)
        if (!m_PageMap[oi])
        {
            usize ii;
            for (ii = 0; ii < count; ++ii)
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

const Bitmap &PageFrameAllocator::GetPageMap() const { return m_PageMap; }

usize PageFrameAllocator::GetFree() const { return m_FreeMemory; }

usize PageFrameAllocator::GetUsed() const { return m_UsedMemory; }

usize PageFrameAllocator::GetReserved() const { return m_ReservedMemory; }

void PageFrameAllocator::InitBitmap(usize size, u8 *buffer)
{
    m_PageMap.Init(size, buffer);
    memset(buffer, 0, size);
}

void PageFrameAllocator::ReservePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (m_PageMap[index])
        return;
    if (!m_PageMap.Set(index, true))
        return;

    m_FreeMemory -= PAGE_SIZE;
    m_ReservedMemory += PAGE_SIZE;
}

void PageFrameAllocator::ReservePages(void *address, usize count)
{
    for (usize off = 0; off < count; ++off)
        ReservePage((void *)((uptr)address + off * PAGE_SIZE));
}

void PageFrameAllocator::UnreservePage(void *address)
{
    auto index = (uptr)address / PAGE_SIZE;
    if (!m_PageMap[index])
        return;
    if (!m_PageMap.Set(index, false))
        return;

    m_FreeMemory += PAGE_SIZE;
    m_ReservedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::UnreservePages(void *address, usize count)
{
    for (usize off = 0; off < count; ++off)
        UnreservePage((void *)((uptr)address + off * PAGE_SIZE));
}
