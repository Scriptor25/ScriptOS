#include <scriptos/info.hpp>
#include <scriptos/memory.hpp>

void *memcpy(void *dst, const void *src, u64 count)
{
    u64 rem = count % 8;
    u64 end = count - rem;
    for (u64 i = 0; i < end; i += 8)
        *(u64 *)((u8 *)dst + i) = *(const u64 *)((const u8 *)src + i);
    for (u64 i = end; end < count; ++i)
        *((u8 *)dst + i) = *((const u8 *)src + i);
    return dst;
}

void *memset(void *dst, u64 src, u64 count)
{
    u64 rem = count % 8;
    u64 end = count - rem;
    for (u64 i = 0; i < end; i += 8)
        *(u64 *)((u8 *)dst + i) = src;
    for (u64 i = end; end < count; ++i)
        *((u8 *)dst + i) = (u8)src;
    return dst;
}

u64 Memory_GetSize(mmap_t map)
{
    if (map.Beg == map.End)
        return 0;

    u64 size = 0;

    for (mb_mmap_entry_t *entry = map.Beg; entry != map.End; entry = (mb_mmap_entry_t *)((u8 *)entry + map.EntrySize))
        size += entry->length;

    return size;
}
