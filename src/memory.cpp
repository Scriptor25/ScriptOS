#include <scriptos/memory.h>

void memory::Fill(
    void* dst,
    int value,
    usize count)
{
    if (!dst || !count)
        return;

    auto word_count = (count / sizeof(int)) * sizeof(int);

    for (unsigned i = 0; i < sizeof(int) * 8; i += 8)
        value |= ((value & 0xff) << i);

    usize i;
    for (i = 0; i < word_count; i += sizeof(int))
        *reinterpret_cast<int*>(reinterpret_cast<uptr>(dst) + i) = value;
    for (; i < count; ++i)
        *reinterpret_cast<u8*>(reinterpret_cast<uptr>(dst) + i) = value;
}

void memory::Copy(
    void* dst,
    const void* src,
    usize count)
{
    if (!dst || !src || !count)
        return;

    auto word_count = (count / sizeof(int)) * sizeof(int);

    usize i;
    for (i = 0; i < word_count; i += sizeof(int))
        *reinterpret_cast<int*>(reinterpret_cast<uptr>(dst) + i) = *reinterpret_cast<const int*>(reinterpret_cast<uptr>(src) + i);
    for (; i < count; ++i)
        *reinterpret_cast<u8*>(reinterpret_cast<uptr>(dst) + i) = *reinterpret_cast<const u8*>(reinterpret_cast<uptr>(src) + i);
}

bool memory::Compare(
    cstr s1,
    cstr s2,
    usize count)
{
    for (usize i = 0; i < count; ++i)
        if (s1[i] != s2[i])
            return false;
    return true;
}
