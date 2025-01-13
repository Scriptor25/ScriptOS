#include <scriptos/memory.hpp>

void *memcpy(void *dst, const void *src, usize count)
{
    auto rem = count % sizeof(int);
    auto end = count - rem;
    for (usize i = 0; i < end; i += sizeof(int))
        *(int *)((uptr)dst + i) = *(const int *)((uptr)src + i);
    for (usize i = end; i < count; ++i)
        *((u8 *)dst + i) = *((const u8 *)src + i);
    return dst;
}

void *memset(void *dst, int src, usize count)
{
    auto rem = count % sizeof(int);
    auto end = count - rem;

    src &= 0xff;
    src |= src << 8 | src << 16 | src << 24;

    for (usize i = 0; i < end; i += sizeof(int))
        *(int *)((uptr)dst + i) = src;
    for (usize i = end; i < count; ++i)
        *((u8 *)dst + i) = (u8)src;
    return dst;
}

void *memset(void *dst, int src, usize size, usize count)
{
    for (usize i = 0; i < count * size; i += size)
        *(int *)((uptr)dst + i) = src;
    return dst;
}
