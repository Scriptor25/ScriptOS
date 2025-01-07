#include <scriptos/bitmap.hpp>

void Bitmap::Init(u64 size, u8 *buffer)
{
    m_Size = size;
    m_Buffer = buffer;
}

u64 Bitmap::GetSize() const
{
    return m_Size;
}

void *Bitmap::GetBuffer() const
{
    return m_Buffer;
}

bool Bitmap::operator[](u64 index) const
{
    return Get(index);
}

bool Bitmap::Get(u64 index) const
{
    u64 byte_index = index / 8;
    u8 bit_index = index % 8;
    u8 bit_mask = 1 << bit_index;
    return m_Buffer[byte_index] & bit_mask;
}

void Bitmap::Set(u64 index, bool value)
{
    u64 byte_index = index / 8;
    u8 bit_index = index % 8;
    u8 bit_mask = 1 << bit_index;
    if (value)
        m_Buffer[byte_index] |= bit_mask;
    else
        m_Buffer[byte_index] &= ~bit_mask;
}
