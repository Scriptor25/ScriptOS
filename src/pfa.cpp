#include <scriptos/paging.h>

kernel::PageFrameAllocator::PageFrameAllocator(const kernel::Bitmap& bitmap)
    : m_Bitmap(bitmap)
{
}

void kernel::PageFrameAllocator::LockPage(const void* address)
{
    m_Bitmap.Set(reinterpret_cast<uptr>(address) / PAGE_SIZE, true);
}

void kernel::PageFrameAllocator::FreePage(const void* address)
{
    m_Bitmap.Set(reinterpret_cast<uptr>(address) / PAGE_SIZE, false);
}

void kernel::PageFrameAllocator::LockPages(
    const void* address,
    usize count)
{
    m_Bitmap.Fill(reinterpret_cast<uptr>(address) / PAGE_SIZE, count, true);
}

void kernel::PageFrameAllocator::FreePages(
    const void* address,
    usize count)
{
    m_Bitmap.Fill(reinterpret_cast<uptr>(address) / PAGE_SIZE, count, false);
}

void* kernel::PageFrameAllocator::AllocatePhysicalPage()
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

void* kernel::PageFrameAllocator::AllocatePhysicalPages(usize count)
{
    usize found   = 0;
    void* address = nullptr;

    for (auto [index, state] : m_Bitmap)
    {
        if (state)
        {
            found   = 0;
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
        {
            continue;
        }

        LockPages(address, found);
        return address;
    }
    return nullptr;
}
