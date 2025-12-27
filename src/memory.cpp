#include <scriptos/memory.h>

void kernel::Fill(
    void* dst,
    u8 value,
    usize count)
{
    if (!dst || !count)
        return;

    auto block_count = (count / sizeof(u64)) * sizeof(u64);

    u64 block = 0;
    for (unsigned i = 0; i < sizeof(u64) * 8; i += 8)
        block |= ((value & 0xff) << i);

    usize i;
    for (i = 0; i < block_count; i += sizeof(u64))
        *reinterpret_cast<u64*>(reinterpret_cast<uptr>(dst) + i) = block;
    for (; i < count; ++i)
        *reinterpret_cast<u8*>(reinterpret_cast<uptr>(dst) + i) = value;
}

void kernel::Copy(
    void* dst,
    const void* src,
    usize count)
{
    if (!dst || !src || !count)
        return;

    auto block_count = (count / sizeof(u64)) * sizeof(u64);

    usize i;
    for (i = 0; i < block_count; i += sizeof(u64))
        *reinterpret_cast<u64*>(reinterpret_cast<uptr>(dst) + i) = *reinterpret_cast<const u64*>(reinterpret_cast<uptr>(src) + i);
    for (; i < count; ++i)
        *reinterpret_cast<u8*>(reinterpret_cast<uptr>(dst) + i) = *reinterpret_cast<const u8*>(reinterpret_cast<uptr>(src) + i);
}

bool kernel::Compare(
    cstr s1,
    cstr s2,
    usize count)
{
    for (usize i = 0; i < count; ++i)
        if (s1[i] != s2[i])
            return false;
    return true;
}
