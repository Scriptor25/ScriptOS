#include <scriptos/mmap.hpp>

MemoryMap::Iter::Iter(const multiboot_mmap_entry *ptr, u32 entry_size)
    : m_Ptr(ptr), m_EntrySize(entry_size) {}

const multiboot_mmap_entry &MemoryMap::Iter::operator*() const { return *m_Ptr; }

MemoryMap::Iter &MemoryMap::Iter::operator++()
{
    m_Ptr = (const multiboot_mmap_entry *)((u8 *)m_Ptr + m_EntrySize);
    return *this;
}

MemoryMap::Iter MemoryMap::Iter::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}

bool MemoryMap::Iter::operator==(const Iter &other) const { return other.m_Ptr == m_Ptr; }
bool MemoryMap::Iter::operator!=(const Iter &other) const { return other.m_Ptr != m_Ptr; }

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

MemoryMap::Iter MemoryMap::begin() const
{
    return Iter(m_Beg, m_EntrySize);
}

MemoryMap::Iter MemoryMap::end() const
{
    return Iter(m_End, m_EntrySize);
}
