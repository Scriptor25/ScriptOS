#pragma once

#include <scriptos/std/assert.hpp>
#include <scriptos/std/types.hpp>

template <typename T>
class view
{
public:
    view()
        : view(nullptr, nullptr)
    {
    }

    view(const T *begin, const T *end)
        : m_Begin(begin), m_End(end)
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

    const T *begin() const { return m_Begin; }
    const T *end() const { return m_End; }

    const T &at(size index) const
    {
        assert(index < m_End - m_Begin);
        return m_Begin[index];
    }

    size size() const { return m_End - m_Begin; }

private:
    const T *m_Begin;
    const T *m_End;
};

using strview = view<char>;
using strview = view<wchar_t>;
