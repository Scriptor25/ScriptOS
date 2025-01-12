#include <scriptos/bitmap.hpp>

Bitmap::Iter::Iter(const Bitmap &bitmap, usize index)
    : m_Bitmap(bitmap), m_Index(index)
{
}

BitEntry Bitmap::Iter::operator*() const
{
    return {
        m_Index / 8,
        m_Index % 8,
        m_Bitmap[m_Index],
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

void Bitmap::Init(usize size, u8 *buffer)
{
    m_Size = size;
    m_Buffer = buffer;
}

usize Bitmap::Size() const
{
    return m_Size;
}

void *Bitmap::Buffer() const
{
    return m_Buffer;
}

bool Bitmap::operator[](usize index) const
{
    return Get(index);
}

bool Bitmap::Get(usize index) const
{
    if (index >= m_Size * 8)
        return false;

    auto byte_index = index / 8;
    auto bit_index = index % 8;
    auto bit_mask = 1 << bit_index;
    return m_Buffer[byte_index] & bit_mask;
}

bool Bitmap::Set(usize index)
{
    if (index >= m_Size * 8)
        return false;

    auto byte_index = index / 8;
    auto bit_index = index % 8;
    auto bit_mask = 1 << bit_index;
    m_Buffer[byte_index] |= bit_mask;
    return true;
}

bool Bitmap::Clr(usize index)
{
    if (index >= m_Size * 8)
        return false;

    auto byte_index = index / 8;
    auto bit_index = index % 8;
    auto bit_mask = 1 << bit_index;
    m_Buffer[byte_index] &= ~bit_mask;
    return true;
}

Bitmap::Iter Bitmap::begin() const
{
    return Iter(*this, 0);
}

Bitmap::Iter Bitmap::end() const
{
    return Iter(*this, m_Size * 8);
}
