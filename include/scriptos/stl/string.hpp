#pragma once

#include <scriptos/stl/vector.hpp>

class string;
class string_view;

class string : public vector<char>
{
public:
    string();
    string(cstr data);
    string(const string_view& data);
    string(cstr begin, cstr end);

    vector<string> split(char value) const;
    string trim() const;
};
