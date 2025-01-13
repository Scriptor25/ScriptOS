#pragma once

#include <scriptos/assert.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

template <typename T>
class vector
{
public:
    vector(usize size = 0, usize reserved = 10)
    {
        assert(size <= reserved);

        m_Size = size;
        m_Reserved = reserved;
        m_Data = (T *)calloc(reserved, sizeof(T));
    }

    vector(T *data, usize size)
        : vector(size, size)
    {
        memcpy(m_Data, data, size);
    }

    vector(T *begin, T *end)
        : vector(begin, end - begin) {}

    vector(const vector &other)
        : vector(other.m_Data, other.m_Size) {}

    vector(vector &&other)
    {
        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Reserved = 0;
    }

    ~vector()
    {
        free(m_Data);
        m_Data = nullptr;
        m_Size = 0;
        m_Reserved = 0;
    }

    vector &operator=(const vector &other)
    {
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;
        m_Data = realloc(m_Data, m_Reserved * sizeof(T));
        memcpy(m_Data, other.m_Data, m_Size);
    }

    vector &operator=(vector &&other)
    {
        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Reserved = 0;

        return *this;
    }

    T &at(usize index)
    {
        return m_Data[index];
    }

    const T &at(usize index) const
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

    T *data()
    {
        return m_Data;
    }

    const T *data() const
    {
        return m_Data;
    }

    usize size() const
    {
        return m_Size;
    }

    usize reserved() const
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

    T &push_back(const T &e)
    {
        if (m_Size + 1 >= m_Reserved)
        {
            m_Reserved += m_Reserved;
            m_Data = (T *)realloc(m_Data, m_Reserved * sizeof(T));
        }
        return m_Data[m_Size++] = e;
    }

    T pop_back()
    {
        return m_Data[--m_Size];
    }

private:
    T *m_Data;
    usize m_Size;
    usize m_Reserved;
};
