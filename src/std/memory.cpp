#include <scriptos/std/memory.hpp>

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

    auto byte = src & 0xff;

    src = 0;
    for (usize i = 0; i < sizeof(int) * 8; i += 8)
        src |= byte << i;

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

int memcmp(const void *ptr1, const void *ptr2, usize count)
{
    for (usize i = 0; i < count; ++i)
    {
        auto diff = ((u8 *)ptr1)[i] - ((u8 *)ptr2)[i];
        if (diff)
            return diff;
    }

    return 0;
}

int strlen(cstr string)
{
    int i = 0;
    for (auto p = (str)string; *p; ++p, ++i)
        ;
    return i;
}

int strlen(cwstr string)
{
    int i = 0;
    for (auto p = (wstr)string; *p; ++p, ++i)
        ;
    return i;
}
