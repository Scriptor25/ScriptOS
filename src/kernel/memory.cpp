#include <scriptos/info.hpp>
#include <scriptos/memory.hpp>

void *memcpy(void *dst, const void *src, u64 count)
{
    u64 rem = count % 8;
    u64 end = count - rem;
    for (u64 i = 0; i < end; i += 8)
        *(u64 *)((u8 *)dst + i) = *(const u64 *)((const u8 *)src + i);
    for (u64 i = end; i < count; ++i)
        *((u8 *)dst + i) = *((const u8 *)src + i);
    return dst;
}

void *memset(void *dst, u64 src, u64 count)
{
    u64 rem = count % 8;
    u64 end = count - rem;
    for (u64 i = 0; i < end; i += 8)
        *(u64 *)((u8 *)dst + i) = src;
    for (u64 i = end; i < count; ++i)
        *((u8 *)dst + i) = (u8)src;
    return dst;
}

MemoryMap::MemoryMap(const multiboot_mmap_entry *beg, const multiboot_mmap_entry *end, const u32 entry_size)
    : m_Beg(beg), m_End(end), m_EntrySize(entry_size)
{
}

Iterator<const multiboot_mmap_entry> MemoryMap::begin() const
{
    return Iterator(m_Beg, m_EntrySize);
}

Iterator<const multiboot_mmap_entry> MemoryMap::end() const
{
    return Iterator(m_End, m_EntrySize);
}

u64 Memory_GetSize(const MemoryMap &mmap)
{
    u64 size = 0;

    for (auto &entry : mmap)
        size += entry.length;

    return size;
}
