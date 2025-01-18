#pragma once

#include <scriptos/std/assert.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

template <typename T>
class vector
{
public:
    using ElementType = T;

    vector(usize size = 0, usize reserved = 10)
        : m_Size(size), m_Reserved(max(size, reserved))
    {
        m_Data = (T *)calloc(m_Reserved, sizeof(T));
    }

    vector(const T *data, usize size)
        : m_Size(size), m_Reserved(size)
    {
        m_Data = (T *)malloc(m_Reserved * sizeof(T));
        memcpy(m_Data, data, m_Size * sizeof(T));
    }

    vector(const T *data)
        : m_Size(strlen(data)), m_Reserved(m_Size)
    {
        m_Data = (T *)malloc(m_Reserved * sizeof(T));
        memcpy(m_Data, data, m_Size * sizeof(T));
    }

    vector(const T *begin, const T *end)
        : m_Size(end - begin), m_Reserved(end - begin)
    {
        m_Data = (T *)malloc(m_Reserved * sizeof(T));
        memcpy(m_Data, begin, m_Size * sizeof(T));
    }

    vector(const vector &other)
        : m_Size(other.m_Size), m_Reserved(other.m_Reserved)
    {
        m_Data = (T *)malloc(m_Reserved * sizeof(T));
        memcpy(m_Data, other.m_Data, m_Size * sizeof(T));
    }

    vector(vector &&other)
        : m_Data(other.m_Data), m_Size(other.m_Size), m_Reserved(other.m_Reserved)
    {
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
        m_Data = (T *)realloc(m_Data, m_Reserved * sizeof(T));
        memcpy(m_Data, other.m_Data, m_Size * sizeof(T));

        return *this;
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
        assert(index < m_Size);
        return m_Data[index];
    }

    const T &at(usize index) const
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    T &operator[](usize index)
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    const T &operator[](usize index) const
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    operator auto()
    {
        return m_Data;
    }

    operator auto() const
    {
        return m_Data;
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

    template <typename... Args>
    T &emplace_back(Args... args)
    {
        T e(args...);
        return push_back(e);
    }

    T &push_back(T &&e)
    {
        if (m_Size + 1 >= m_Reserved)
        {
            m_Reserved += m_Reserved;
            m_Data = (T *)realloc(m_Data, m_Reserved * sizeof(T));
        }
        return m_Data[m_Size++] = e;
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

    T pop_front()
    {
        auto element = m_Data[0];
        m_Size--;
        auto buffer = (T *)malloc(m_Reserved * sizeof(T));
        memcpy(buffer, m_Data + 1, m_Size * sizeof(T));
        free(m_Data);
        m_Data = buffer;
        return element;
    }

    T pop_back()
    {
        return m_Data[--m_Size];
    }

    vector<vector> split(const T &value) const
    {
        vector<vector> elements;
        usize begin = 0;

        for (usize i = 0; i < m_Size; ++i)
            if (at(i) == value)
            {
                if (i - begin)
                    elements.emplace_back(m_Data + begin, m_Data + i);
                begin = i + 1;
            }

        if (begin < m_Size)
            elements.emplace_back(m_Data + begin, m_Data + m_Size);

        return elements;
    }

    vector trim() const
    {
        auto b = begin();
        for (; b < end() && isspace(*b); ++b)
            ;

        auto e = end() - 1;
        for (; e >= begin() && isspace(*e); --e)
            ;

        return vector(b, e + 1);
    }

private:
    T *m_Data;
    usize m_Size;
    usize m_Reserved;
};

using string = vector<char>;
using wstring = vector<wchar_t>;
