#include <scriptos/bitmap.h>
#include <scriptos/memory.h>

Bitmap::Iterator::Iterator(const Bitmap& bitmap, usize index)
    : m_Bitmap(bitmap), m_Index(index)
{
}

Bitmap::Entry Bitmap::Iterator::operator*() const
{
    return {
        .Index = m_Index,
        .State = m_Bitmap.Get(m_Index),
    };
}

Bitmap::Iterator& Bitmap::Iterator::operator++()
{
    m_Index++;
    return *this;
}

bool Bitmap::Iterator::operator!=(const Iterator& other) const
{
    return m_Index != other.m_Index;
}

Bitmap::Bitmap(u8* buffer, usize size_in_bits)
    : m_Buffer(buffer), m_SizeInBits(size_in_bits & ~7)
{
}

void Bitmap::Clear()
{
    memory::Fill(m_Buffer, 0, m_SizeInBits / 8);
}

void Bitmap::Set(usize index, bool state)
{
    if (index >= m_SizeInBits)
        return;

    auto byte_index = index / 8;
    auto bit_index = index % 8;
    u8 mask = 1 << bit_index;

    if (state)
        m_Buffer[byte_index] |= mask;
    else
        m_Buffer[byte_index] &= ~mask;
}

void Bitmap::Fill(usize index, usize count, bool state)
{
    if (index >= m_SizeInBits)
        return;

    if (index + count >= m_SizeInBits)
        count = m_SizeInBits - index;

    for (usize i = 0; i < count; ++i)
        Set(index + i, state);
}

bool Bitmap::Get(usize index) const
{
    auto byte_index = index / 8;
    auto bit_index = index % 8;
    u8 mask = 1 << bit_index;

    return m_Buffer[byte_index] & mask;
}

Bitmap::Iterator Bitmap::begin() const
{
    return { *this, 0 };
}

Bitmap::Iterator Bitmap::end() const
{
    return { *this, m_SizeInBits };
}
