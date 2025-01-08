#include <scriptos/bitmap.hpp>

Bitmap::Iter::Iter(const Bitmap &bitmap, u64 index)
    : m_Bitmap(bitmap), m_Index(index)
{
}

BitEntry Bitmap::Iter::operator*() const
{
    auto byte_index = m_Index / 8;
    auto bit_index = m_Index % 8;
    auto active = m_Bitmap.Get(m_Index);
    return {
        byte_index,
        bit_index,
        active,
    };
}

Bitmap::Iter &Bitmap::Iter::operator++()
{
    m_Index++;
    return *this;
}

Bitmap::Iter Bitmap::Iter::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}

bool Bitmap::Iter::operator==(const Iter &other) const { return other.m_Index == m_Index; }

bool Bitmap::Iter::operator!=(const Iter &other) const { return other.m_Index != m_Index; }

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
    auto byte_index = index / 8;
    auto bit_index = index % 8;
    auto bit_mask = 1 << bit_index;
    return m_Buffer[byte_index] & bit_mask;
}

void Bitmap::Set(u64 index, bool value)
{
    auto byte_index = index / 8;
    auto bit_index = index % 8;
    auto bit_mask = 1 << bit_index;
    if (value)
        m_Buffer[byte_index] |= bit_mask;
    else
        m_Buffer[byte_index] &= ~bit_mask;
}

Bitmap::Iter Bitmap::begin() const
{
    return Iter(*this, 0);
}

Bitmap::Iter Bitmap::end() const
{
    return Iter(*this, m_Size);
}
