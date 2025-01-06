#pragma once

#include <scriptos/types.hpp>

template <typename T>
class Iterator
{
public:
    Iterator(T *ptr, u64 stride)
        : m_Ptr(ptr), m_Stride(stride) {}

    T &operator*() const { return *m_Ptr; }

    Iterator &operator++()
    {
        m_Ptr = (T *)((u8 *)m_Ptr + m_Stride);
        return *this;
    }

    Iterator operator++(int)
    {
        auto old = *this;
        ++(*this);
        return old;
    }

    bool operator==(const Iterator &other) const { return other.m_Ptr == m_Ptr; }
    bool operator!=(const Iterator &other) const { return other.m_Ptr != m_Ptr; }

private:
    T *m_Ptr;
    u64 m_Stride;
};
