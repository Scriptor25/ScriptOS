#pragma once

#include <scriptos/assert.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/paging.hpp>
#include <scriptos/types.hpp>

template <typename T>
class Vec
{
public:
    Vec(usize size = 0, usize reserved = 10)
    {
        assert(size <= reserved);

        m_Size = size;
        m_Reserved = reserved;
        m_Data = calloc(reserved, sizeof(T));
    }

    Vec(T *data, usize size)
        : Vec(size, size)
    {
        memcpy(m_Data, data, size);
    }

    Vec(T *begin, T *end)
        : Vec(begin, end - begin) {}

    Vec(const Vec &other)
        : Vec(other.m_Data, other.m_Size) {}

    Vec(Vec &&other)
    {
        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Reserved = 0;
    }

    ~Vec()
    {
        free(m_Data);
        m_Data = nullptr;
        m_Size = 0;
        m_Reserved = 0;
    }

    Vec &operator=(const Vec &other)
    {
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;
        m_Data = realloc(m_Data, m_Reserved * sizeof(T));
        memcpy(m_Data, other.m_Data, m_Size);
    }

    Vec &operator=(Vec &&other)
    {
        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Reserved = 0;

        return *this;
    }

    T &At(usize index)
    {
        return m_Data[index];
    }

    const T &At(usize index) const
    {
        return m_Data[index];
    }

    T &operator[](usize index)
    {
        return m_Data[index];
    }

    const T &operator[](usize index) const
    {
        return m_Data[index];
    }

    T *Data()
    {
        return m_Data;
    }

    const T *Data() const
    {
        return m_Data;
    }

    usize Size() const
    {
        return m_Size;
    }

    usize Reserved() const
    {
        return m_Reserved;
    }

    T *begin()
    {
        return m_Data;
    }

    const T *begin() const
    {
        return m_Data;
    }

    T *end()
    {
        return m_Data + m_Size;
    }

    const T *end() const
    {
        return m_Data + m_Size;
    }

private:
    T *m_Data;
    usize m_Size;
    usize m_Reserved;
};
