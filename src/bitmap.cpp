#include <scriptos/bitmap.h>
#include <scriptos/memory.h>

kernel::Bitmap::Iterator::Iterator(
    const Bitmap& bitmap,
    usize index)
    : m_bitmap(bitmap),
      m_index(index)
{
}

kernel::Bitmap::Entry kernel::Bitmap::Iterator::operator*() const
{
    return {
        .index = m_index,
        .state = m_bitmap.Get(m_index),
    };
}

kernel::Bitmap::Iterator& kernel::Bitmap::Iterator::operator++()
{
    m_index++;
    return *this;
}

bool kernel::Bitmap::Iterator::operator!=(const Iterator& other) const
{
    return m_index != other.m_index;
}

kernel::Bitmap::Bitmap(
    u8* buffer,
    usize size_in_bits)
    : m_Buffer(buffer),
      m_SizeBits(size_in_bits & ~7)
{
}

void kernel::Bitmap::Clear()
{
    memory::Fill(m_Buffer, 0, m_SizeBits / 8);
}

void kernel::Bitmap::Set(
    usize index,
    bool state)
{
    if (index >= m_SizeBits)
    {
        return;
    }

    auto byte_index = index / 8;
    auto bit_index = index % 8;
    u8 mask = 1 << bit_index;

    if (state)
    {
        m_Buffer[byte_index] |= mask;
    }
    else
    {
        m_Buffer[byte_index] &= ~mask;
    }
}

void kernel::Bitmap::Fill(
    usize index,
    usize count,
    bool state)
{
    if (index >= m_SizeBits)
    {
        return;
    }

    if (index + count >= m_SizeBits)
    {
        count = m_SizeBits - index;
    }

    for (usize i = 0; i < count; ++i)
    {
        Set(index + i, state);
    }
}

bool kernel::Bitmap::Get(usize index) const
{
    auto byte_index = index / 8;
    auto bit_index = index % 8;
    u8 mask = 1 << bit_index;

    return m_Buffer[byte_index] & mask;
}

kernel::Bitmap::Iterator kernel::Bitmap::begin() const
{
    return { *this, 0 };
}

kernel::Bitmap::Iterator kernel::Bitmap::end() const
{
    return { *this, m_SizeBits };
}
