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
