#pragma once

#include <scriptos/memory.h>
#include <scriptos/types.h>

template<typename T>
class vector;

template<typename T>
class view;

template<typename T>
class vector
{
public:
    vector(usize size = 10)
    {
        m_First = memory::Allocate<T>(size);
        m_Last = m_First;
        m_End = m_First + size;
    }

    vector(
        const T* begin,
        const T* end)
        : vector(end - begin)
    {
        auto size = end - begin;
        auto count = size * sizeof(T);
        memory::Copy(m_First, begin, count);
        m_Last = m_First + size;
    }

    vector(
        const T* buffer,
        usize size)
        : vector(
              buffer,
              buffer + size)
    {
    }

    vector(const vector& v)
        : vector(
              v.begin(),
              v.end())
    {
    }

    vector(vector&& v) noexcept
        : m_First(v.m_First),
          m_Last(v.m_Last),
          m_End(v.m_End)
    {
        v.m_First = v.m_Last = v.m_End = nullptr;
    }

    ~vector()
    {
        for (auto p = m_First; p < m_Last; ++p)
            p->~T();
        memory::Free(m_First);
        m_First = m_Last = m_End = nullptr;
    }

    vector& operator=(const vector& v)
    {
        auto size = v.m_Last - v.m_First;
        auto count = size * sizeof(T);

        m_First = memory::Reallocate<T>(m_First, size);

        memory::Copy(m_First, v.m_First, count);

        m_Last = m_End = m_First + size;

        return *this;
    }

    vector& operator=(vector&& v) noexcept
    {
        memory::Swap(m_First, v.m_First);
        memory::Swap(m_Last, v.m_Last);
        memory::Swap(m_End, v.m_End);
        return *this;
    }

    usize size() const { return m_Last - m_First; }
    bool empty() const { return m_First == m_Last; }

    T* data() { return m_First; }
    const T* data() const { return m_First; }

    T* begin() { return m_First; }
    T* end() { return m_Last; }

    const T* begin() const { return m_First; }
    const T* end() const { return m_Last; }

    T& front() { return *m_First; }
    T& back() { return *(m_Last - 1); }

    const T& front() const { return *m_First; }
    const T& back() const { return *(m_Last - 1); }

    T& at(usize index) { return m_First[index]; }
    const T& at(usize index) const { return m_First[index]; }

    T& operator[](usize index) { return m_First[index]; }
    const T& operator[](usize index) const { return m_First[index]; }

    void push_back(const T& e)
    {
        expand();
        *(m_Last++) = e;
    }

    void push_back(T&& e)
    {
        expand();
        *(m_Last++) = memory::Move(e);
    }

    void push_range(
        const T* begin,
        const T* end)
    {
        auto size = end - begin;
        if (m_Last + size >= m_End)
        {
            auto missing = size - (m_End - m_Last);
            reserve(missing);
        }

        for (auto p = begin; p < end; ++p)
            *(m_Last++) = *p;
    }

    T& emplace_back(const T& e)
    {
        expand();
        return *(m_Last++) = e;
    }

    T& emplace_back(T&& e)
    {
        expand();
        return *(m_Last++) = memory::Move(e);
    }

    template<typename... I>
    T& emplace_back(I&&... initializer)
    {
        expand();
        return *(m_Last++) = T(memory::Move(initializer)...);
    }

    void reserve(usize size)
    {
        auto elements = m_Last - m_First;
        auto reserved = m_End - m_First;
        auto extended = reserved + size;

        m_First = memory::Reallocate<T>(m_First, extended);
        m_Last = m_First + elements;
        m_End = m_First + extended;
    }

    void resize(usize size)
    {
        auto reserved = m_End - m_First;
        if (size > reserved)
            reserve(size - reserved);

        m_Last = m_First + size;
        return;
    }

    void expand()
    {
        if (m_Last < m_End)
            return;

        auto reserved = m_First - m_End;
        reserve(reserved);
    }

protected:
    T *m_First, *m_Last, *m_End;
};

template<typename T>
class view
{
public:
    view()
        : m_First(nullptr),
          m_Last(nullptr)
    {
    }

    view(
        const T* begin,
        const T* end)
        : m_First(begin),
          m_Last(end)
    {
    }

    view(const vector<T>& v)
        : view(
              v.begin(),
              v.end())
    {
    }

    const T* begin() const { return m_First; }
    const T* end() const { return m_Last; }

protected:
    const T *m_First, *m_Last;
};

template<typename T = char>
class string : public vector<T>
{
public:
    string(usize size = 10)
        : vector<T>(size)
    {
    }

    string(const T* value)
        : vector<T>(
              value,
              memory::StringLength(value))
    {
    }

    string& operator+=(T value)
    {
        push_back(value);
        return *this;
    }

    string& operator+=(const T* value)
    {
        push_range(value, value + memory::StringLength(value));
        return *this;
    }

    string& operator+=(const string& value)
    {
        push_range(value.begin(), value.end());
        return *this;
    }
};
