#include <scriptos/kernel/mmap.hpp>

MemoryMap::Iter::Iter(const multiboot_mmap_entry *ptr, usize entry_size)
    : m_Ptr(ptr), m_EntrySize(entry_size)
{
}

const multiboot_mmap_entry &MemoryMap::Iter::operator*() const { return *m_Ptr; }

MemoryMap::Iter &MemoryMap::Iter::operator++()
{
    m_Ptr = reinterpret_cast<const multiboot_mmap_entry *>(reinterpret_cast<const u8 *>(m_Ptr) + m_EntrySize);
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
    : m_Beg(nullptr), m_End(nullptr), m_EntrySize(0), m_Size(0)
{
}

MemoryMap::MemoryMap(const multiboot_mmap_entry *beg, const multiboot_mmap_entry *end, usize entry_size)
    : m_Beg(beg), m_End(end), m_EntrySize(entry_size), m_Size(0)
{
    for (auto &entry : *this)
    {
        if (entry.base_addr_hi || entry.length_hi)
            break;
        m_Size += entry.length_lo;
    }
}

usize MemoryMap::Size() const { return m_Size; }

MemoryMap::Iter MemoryMap::begin() const
{
    return Iter(m_Beg, m_EntrySize);
}

MemoryMap::Iter MemoryMap::end() const
{
    return Iter(m_End, m_EntrySize);
}
