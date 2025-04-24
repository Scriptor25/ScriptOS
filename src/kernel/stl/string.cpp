#include <scriptos/stl/string.hpp>
#include <scriptos/stl/string_view.hpp>

string::string()
  : vector()
{
}

string::string(cstr data)
  : vector(data, strlen(data))
{
}

string::string(const string_view& data)
  : vector(data.begin(), data.end())
{
}

string::string(cstr begin, cstr end)
  : vector(begin, end)
{
}

vector<string> string::split(char value) const
{
    vector<string> elements;
    usize beg = 0;

    auto count = size();
    for (usize i = 0; i < count; ++i)
        if (at(i) == value)
        {
            if (i - beg)
                elements.emplace_back(begin() + beg, begin() + i);
            beg = i + 1;
        }

    if (beg < count)
        elements.emplace_back(begin() + beg, end());

    return elements;
}

string string::trim() const
{
    auto b = begin();
    for (; b < end() && isspace(*b); ++b)
        ;

    auto e = end() - 1;
    for (; e >= begin() && isspace(*e); --e)
        ;

    return string(b, e + 1);
}
