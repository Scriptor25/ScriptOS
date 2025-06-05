#pragma once

#include <scriptos/types.h>

template<typename T>
class Range
{
public:
    Range(T* buffer, usize size)
        : m_Buffer(buffer), m_Size(size)
    {
    }

    T* begin() const { return m_Buffer; }
    T* end() const { return m_Buffer + m_Size; }

private:
    T* m_Buffer;
    usize m_Size;
};
