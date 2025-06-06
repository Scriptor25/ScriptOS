#pragma once

#include <scriptos/std/assert.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>
#include <scriptos/stl/util.hpp>

template<typename T>
class view;

template<typename T>
class vector
{
public:
    using ElementType = T;

    vector(usize size = 0, usize reserved = 10)
      : m_Size(size), m_Reserved(max(size, reserved))
    {
        m_Data = reinterpret_cast<T*>(malloc(m_Reserved * sizeof(T)));
        memset(m_Data, 0, m_Reserved * sizeof(T));
    }

    vector(const T* data, usize size)
      : m_Size(size), m_Reserved(size)
    {
        m_Data = reinterpret_cast<T*>(malloc(m_Reserved * sizeof(T)));
        memcpy(m_Data, data, m_Size * sizeof(T));
    }

    vector(const T* begin, const T* end)
      : m_Size(end - begin), m_Reserved(end - begin)
    {
        m_Data = reinterpret_cast<T*>(malloc(m_Reserved * sizeof(T)));
        memcpy(m_Data, begin, m_Size * sizeof(T));
    }

    vector(const vector& other)
      : m_Size(other.m_Size), m_Reserved(other.m_Reserved)
    {
        m_Data = reinterpret_cast<T*>(malloc(m_Reserved * sizeof(T)));
        memcpy(m_Data, other.m_Data, m_Size * sizeof(T));
    }

    vector(vector&& other)
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

    vector& operator=(const vector& other)
    {
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;
        m_Data = reinterpret_cast<T*>(realloc(m_Data, m_Reserved * sizeof(T)));
        memcpy(m_Data, other.m_Data, m_Size * sizeof(T));

        return *this;
    }

    vector& operator=(vector&& other)
    {
        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Reserved = other.m_Reserved;

        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Reserved = 0;

        return *this;
    }

    vector& operator+=(const T& other)
    {
        push_back(other);
        return *this;
    }

    vector& operator+=(const vector& other) { return *this += view(other); }

    vector& operator+=(const view<T>& other)
    {
        other.size();

        auto start = m_Size;
        m_Size += other.size();
        if (m_Reserved < m_Size)
        {
            do
            {
                m_Reserved += m_Reserved;
            } while (m_Reserved < m_Size);
            m_Data = reinterpret_cast<T*>(realloc(m_Data, m_Reserved * sizeof(T)));
        }

        memcpy(m_Data + start, other.data(), other.size());
        return *this;
    }

    T& at(usize index)
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    const T& at(usize index) const
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    T& operator[](usize index)
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    const T& operator[](usize index) const
    {
        assert(index < m_Size);
        return m_Data[index];
    }

    T* data() { return m_Data; }

    const T* data() const { return m_Data; }

    usize size() const { return m_Size; }

    usize reserved() const { return m_Reserved; }

    bool empty() const { return m_Size == 0; }

    T* begin() { return m_Data; }

    const T* begin() const { return m_Data; }

    T* end() { return m_Data + m_Size; }

    const T* end() const { return m_Data + m_Size; }

    T& front()
    {
        assert(m_Size > 0);
        return m_Data[0];
    }

    const T& front() const
    {
        assert(m_Size > 0);
        return m_Data[0];
    }

    T& back()
    {
        assert(m_Size > 0);
        return m_Data[m_Size - 1];
    }

    const T& back() const
    {
        assert(m_Size > 0);
        return m_Data[m_Size - 1];
    }

    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        return push_back(T(args...));
    }

    T& push_back(T&& e)
    {
        if (m_Reserved <= m_Size)
        {
            m_Reserved += m_Reserved;
            m_Data = reinterpret_cast<T*>(realloc(m_Data, m_Reserved * sizeof(T)));
        }
        return m_Data[m_Size++] = move(e);
    }

    T& push_back(const T& e)
    {
        if (m_Reserved <= m_Size)
        {
            m_Reserved += m_Reserved;
            m_Data = reinterpret_cast<T*>(realloc(m_Data, m_Reserved * sizeof(T)));
        }
        return m_Data[m_Size++] = e;
    }

    T pop_front()
    {
        auto element = m_Data[0];
        m_Size--;
        auto buffer = reinterpret_cast<T*>(malloc(m_Reserved * sizeof(T)));
        memcpy(buffer, m_Data + 1, m_Size * sizeof(T));
        free(m_Data);
        m_Data = buffer;
        return element;
    }

    T pop_back() { return m_Data[--m_Size]; }

    vector& clear()
    {
        m_Size = 0;
        return *this;
    }

private:
    T* m_Data;
    usize m_Size;
    usize m_Reserved;
};
