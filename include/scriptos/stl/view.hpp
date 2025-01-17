#pragma once

#include <scriptos/std/assert.hpp>
#include <scriptos/std/types.hpp>

template <typename T>
class view
{
public:
    view()
        : m_Begin(nullptr), m_End(nullptr)
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

    view(const view &v)
        : m_Begin(v.m_Begin), m_End(v.m_End)
    {
    }

    view(view &&v)
        : m_Begin(v.m_Begin), m_End(v.m_End)
    {
        v.m_Begin = nullptr;
        v.m_End = nullptr;
    }

    ~view()
    {
        m_Begin = nullptr;
        m_End = nullptr;
    }

    const T *begin() const { return m_Begin; }
    const T *end() const { return m_End; }

    const T &at(usize index) const
    {
        assert(index < m_End - m_Begin);
        return m_Begin[index];
    }

    usize size() const { return m_End - m_Begin; }

private:
    const T *m_Begin;
    const T *m_End;
};

using string_view = view<char>;
using wstring_view = view<wchar_t>;
