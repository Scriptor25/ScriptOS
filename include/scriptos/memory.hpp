#pragma once

#include <scriptos/iterator.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/types.hpp>

void *memcpy(void *dst, const void *src, u64 num);
void *memset(void *dst, u64 src, u64 num);

class MemoryMap
{
public:
    MemoryMap(const mb_mmap_entry_t *beg, const mb_mmap_entry_t *end, u32 entry_size);

    Iterator<const mb_mmap_entry_t> begin() const;
    Iterator<const mb_mmap_entry_t> end() const;

private:
    const mb_mmap_entry_t *m_Beg;
    const mb_mmap_entry_t *m_End;
    u32 m_EntrySize;
};

u64 Memory_GetSize(const MemoryMap &mmap);
