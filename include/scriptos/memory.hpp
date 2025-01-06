#pragma once

#include <scriptos/multiboot2.hpp>
#include <scriptos/types.hpp>

void *memcpy(void *dst, const void *src, u64 num);
void *memset(void *dst, u64 src, u64 num);

typedef struct mmap
{
    mb_mmap_entry_t *Beg;
    mb_mmap_entry_t *End;
    u32 EntrySize;
} mmap_t;

u64 Memory_GetSize(mmap_t map);
