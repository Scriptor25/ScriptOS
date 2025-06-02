#include <scriptos/bitmap.h>
#include <scriptos/memory.h>

Bitmap::Bitmap(u8* buffer, usize size_in_bits)
  : m_Buffer(buffer), m_SizeInBits(size_in_bits & ~7)
{
}

void Bitmap::Clear()
{
    memory::fill(m_Buffer, 0, m_SizeInBits / 8);
}

void Bitmap::Set(usize index, bool state)
{
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
