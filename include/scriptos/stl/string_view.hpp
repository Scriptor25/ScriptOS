#pragma once

#include <scriptos/stl/vector.hpp>
#include <scriptos/stl/view.hpp>

class string;
class string_view;

class string_view : public view<char>
{
public:
    string_view();
    string_view(cstr data);
    string_view(const string& data);
    string_view(cstr begin, cstr end);

    vector<string_view> split(char value) const;
    string_view trim() const;
};
