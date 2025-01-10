#pragma once

#include <scriptos/iterator.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/types.hpp>

void *memcpy(void *dst, const void *src, usize num);
void *memset(void *dst, int src, usize num);

class MemoryMap
{
public:
    MemoryMap();
    MemoryMap(const multiboot_mmap_entry *beg, const multiboot_mmap_entry *end, u32 entry_size);

    Iterator<const multiboot_mmap_entry> begin() const;
    Iterator<const multiboot_mmap_entry> end() const;

private:
    const multiboot_mmap_entry *m_Beg;
    const multiboot_mmap_entry *m_End;
    u32 m_EntrySize;
};

u32 Memory_GetSize(const MemoryMap &mmap);
