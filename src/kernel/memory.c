#include <scriptos/memory.h>

void *memcpy(void *dst, const void *src, u64 count)
{
    u64 rem = count % 8;
    u64 end = count - rem;
    for (u64 i = 0; i < end; i += 8)
    {
        *(u64 *)(dst + i) = *(u64 *)(src + i);
    }
    for (u64 i = end; end < count; ++i)
    {
        *(u8 *)(dst + i) = *(u8 *)(src + i);
    }
    return dst;
}

void *memset(void *dst, u64 src, u64 count)
{
    u64 rem = count % 8;
    u64 end = count - rem;
    for (u64 i = 0; i < end; i += 8)
    {
        *(u64 *)(dst + i) = src;
    }
    for (u64 i = end; end < count; ++i)
    {
        *(u8 *)(dst + i) = (u8)src;
    }
    return dst;
}

void *malloc(u64 size)
{
    (void)size;
    return NULL;
}

void *calloc(u64 num, u64 size)
{
    (void)num;
    (void)size;
    return NULL;
}

void *realloc(void *block, u64 size)
{
    (void)block;
    (void)size;
    return NULL;
}

void *free(void *block)
{
    (void)block;
    return NULL;
}
