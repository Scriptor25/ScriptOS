#pragma once

#include <scriptos/memory.h>
#include <scriptos/types.h>

template<typename T>
class Vector;

template<typename T>
class View;

template<typename K, typename V>
struct Pair
{
    K Index;
    V Value;
};

template<typename T>
class Vector
{
public:
    Vector(usize size = 10)
    {
        m_First = kernel::AllocateArray<T>(size);
        m_Last  = m_First;
        m_End   = m_First + size;
    }

    Vector(
        const T* begin,
        const T* end)
        : Vector(end - begin)
    {
        auto size  = end - begin;
        auto count = size * sizeof(T);
        kernel::Copy(m_First, begin, count);
        m_Last = m_First + size;
    }

    Vector(
        const T* buffer,
        usize size)
        : Vector(
              buffer,
              buffer + size)
    {
    }

    Vector(const Vector& v)
        : Vector(
              v.begin(),
              v.end())
    {
    }

    Vector(Vector&& v) noexcept
        : m_First(v.m_First),
          m_Last(v.m_Last),
          m_End(v.m_End)
    {
        v.m_First = v.m_Last = v.m_End = nullptr;
    }

    ~Vector()
    {
        for (auto p = m_First; p < m_Last; ++p)
            p->~T();
        kernel::Free(m_First);
        m_First = m_Last = m_End = nullptr;
    }

    Vector& operator=(const Vector& v)
    {
        auto size  = v.m_Last - v.m_First;
        auto count = size * sizeof(T);

        m_First = kernel::Reallocate<T>(m_First, size);

        kernel::Copy(m_First, v.m_First, count);

        m_Last = m_End = m_First + size;

        return *this;
    }

    Vector& operator=(Vector&& v) noexcept
    {
        kernel::Swap(m_First, v.m_First);
        kernel::Swap(m_Last, v.m_Last);
        kernel::Swap(m_End, v.m_End);
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
        *(m_Last++) = kernel::Move(e);
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
        return *(m_Last++) = kernel::Move(e);
    }

    template<typename... I>
    T& emplace_back(I&&... initializer)
    {
        expand();
        return *(m_Last++) = T(kernel::Move(initializer)...);
    }

    void reserve(usize size)
    {
        auto elements = m_Last - m_First;
        auto reserved = m_End - m_First;
        auto extended = reserved + size;

        m_First = kernel::Reallocate<T>(m_First, extended);
        m_Last  = m_First + elements;
        m_End   = m_First + extended;
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
class View
{
public:
    View()
        : m_First(nullptr),
          m_Last(nullptr)
    {
    }

    View(
        const T* begin,
        const T* end)
        : m_First(begin),
          m_Last(end)
    {
    }

    View(const Vector<T>& v)
        : View(
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
class String : public Vector<T>
{
public:
    String(usize size = 10)
        : Vector<T>(size)
    {
    }

    String(const T* value)
        : Vector<T>(
              value,
              kernel::StringLength(value))
    {
    }

    String& operator+=(T value)
    {
        push_back(value);
        return *this;
    }

    String& operator+=(const T* value)
    {
        push_range(value, value + kernel::StringLength(value));
        return *this;
    }

    String& operator+=(const String& value)
    {
        push_range(value.begin(), value.end());
        return *this;
    }
};

template<typename T = char>
class StringView : public View<T>
{
public:
    StringView()
        : View<T>()
    {
    }

    StringView(
        const T* begin,
        const T* end)
        : View<T>(
              begin,
              end)
    {
    }

    StringView(const Vector<T>& v)
        : View<T>(v)
    {
    }
};
