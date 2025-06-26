#include <scriptos/paging.h>

paging::PageFrameAllocator::PageFrameAllocator(const Bitmap& bitmap)
    : m_Bitmap(bitmap)
{
}

void paging::PageFrameAllocator::LockPage(const void* address)
{
    m_Bitmap.Set(reinterpret_cast<uptr>(address) / PAGE_SIZE, true);
}

void paging::PageFrameAllocator::FreePage(const void* address)
{
    m_Bitmap.Set(reinterpret_cast<uptr>(address) / PAGE_SIZE, false);
}

void paging::PageFrameAllocator::LockPages(
    const void* address,
    usize count)
{
    m_Bitmap.Fill(reinterpret_cast<uptr>(address) / PAGE_SIZE, count, true);
}

void paging::PageFrameAllocator::FreePages(
    const void* address,
    usize count)
{
    m_Bitmap.Fill(reinterpret_cast<uptr>(address) / PAGE_SIZE, count, false);
}

void* paging::PageFrameAllocator::AllocatePhysicalPage()
{
    for (auto [index, state] : m_Bitmap)
        if (!state)
        {
            auto address = reinterpret_cast<void*>(index * PAGE_SIZE);
            LockPage(address);
            return address;
        }
    return nullptr;
}

void* paging::PageFrameAllocator::AllocatePhysicalPages(usize count)
{
    usize found = 0;
    void* address = nullptr;

    for (auto [index, state] : m_Bitmap)
    {
        if (state)
        {
            found = 0;
            address = nullptr;
            continue;
        }

        if (!found)
        {
            address = reinterpret_cast<void*>(index * PAGE_SIZE);
            ++found;
            continue;
        }

        if (++found < count)
            continue;

        LockPages(address, found);
        return address;
    }
    return nullptr;
}

paging::PageFrameAllocator* paging::KernelAllocator;
