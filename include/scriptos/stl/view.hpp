#pragma once

#include <scriptos/std/assert.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/stl/vector.hpp>

template <typename T>
class view
{
public:
    using ElementType = T;

    view()
        : m_Begin(nullptr), m_End(nullptr)
    {
    }

    view(const T *ptr)
        : m_Begin(ptr), m_End(ptr + strlen(ptr))
    {
    }

    view(const T *begin, const T *end)
        : m_Begin(begin), m_End(end)
    {
    }

    view(const T *begin, usize count)
        : m_Begin(begin), m_End(begin + count)
    {
    }

    view(const view &other)
        : m_Begin(other.m_Begin), m_End(other.m_End)
    {
    }

    view(view &&other)
        : m_Begin(other.m_Begin), m_End(other.m_End)
    {
        other.m_Begin = nullptr;
        other.m_End = nullptr;
    }

    ~view()
    {
        m_Begin = nullptr;
        m_End = nullptr;
    }

    view &operator=(const view &other)
    {
        m_Begin = other.m_Begin;
        m_End = other.m_End;

        return *this;
    }

    view &operator=(view &&other)
    {
        m_Begin = other.m_Begin;
        m_End = other.m_End;

        other.m_Begin = nullptr;
        other.m_End = nullptr;

        return *this;
    }

    const T *begin() const { return m_Begin; }
    const T *end() const { return m_End; }

    const T &at(usize index) const
    {
        assert(index < (usize)(m_End - m_Begin));
        return m_Begin[index];
    }

    const T &operator[](usize index) const
    {
        assert(index < (usize)(m_End - m_Begin));
        return m_Begin[index];
    }

    bool operator==(const view &other) const
    {
        if (this == &other)
            return true;
        if (m_Begin == other.m_Begin && m_End == other.m_End)
            return true;
        if (size() != other.size())
            return false;
        return !memcmp(m_Begin, other.m_Begin, size());
    }

    const T *data() const { return m_Begin; }

    usize size() const { return (usize)(m_End - m_Begin); }

    vector<view> split(const T &value) const
    {
        vector<view> elements;
        usize begin = 0;

        auto count = size();
        for (usize i = 0; i < count; ++i)
            if (at(i) == value)
            {
                if (i - begin)
                    elements.emplace_back(m_Begin + begin, m_Begin + i);
                begin = i + 1;
            }

        if (begin < count)
            elements.emplace_back(m_Begin + begin, m_End);

        return elements;
    }

    view trim() const
    {
        auto b = begin();
        for (; b < end() && isspace(*b); ++b)
            ;

        auto e = end() - 1;
        for (; e >= begin() && isspace(*e); --e)
            ;

        return view(b, e + 1);
    }

private:
    const T *m_Begin;
    const T *m_End;
};

using string_view = view<char>;
using wstring_view = view<wchar_t>;
