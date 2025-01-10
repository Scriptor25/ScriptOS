#include <scriptos/info.hpp>
#include <scriptos/memory.hpp>

void *memcpy(void *dst, const void *src, usize count)
{
    auto rem = count % 8;
    auto end = count - rem;
    for (usize i = 0; i < end; i += sizeof(int))
        *(int *)((u8 *)dst + i) = *(const int *)((const u8 *)src + i);
    for (usize i = end; i < count; ++i)
        *((u8 *)dst + i) = *((const u8 *)src + i);
    return dst;
}

void *memset(void *dst, int src, usize count)
{
    usize rem = count % 8;
    usize end = count - rem;
    for (usize i = 0; i < end; i += sizeof(int))
        *(int *)((u8 *)dst + i) = src;
    for (usize i = end; i < count; ++i)
        *((u8 *)dst + i) = (u8)src;
    return dst;
}

MemoryMap::MemoryMap()
    : m_Beg(nullptr), m_End(nullptr), m_EntrySize(0)
{
}

MemoryMap::MemoryMap(const multiboot_mmap_entry *beg, const multiboot_mmap_entry *end, const u32 entry_size)
    : m_Beg(beg), m_End(end), m_EntrySize(entry_size)
{
}

u64 MemoryMap::Size()
{
    return m_Size = ((const MemoryMap *)this)->Size();
}

u64 MemoryMap::Size() const
{
    if (m_Size)
        return m_Size;
    u64 size = 0;
    for (auto &entry : *this)
    {
        if (entry.base_addr + entry.length > 0xffffffff)
            break;
        size += entry.length;
    }
    return size;
}

Iterator<const multiboot_mmap_entry> MemoryMap::begin() const
{
    return Iterator(m_Beg, m_EntrySize);
}

Iterator<const multiboot_mmap_entry> MemoryMap::end() const
{
    return Iterator(m_End, m_EntrySize);
}
