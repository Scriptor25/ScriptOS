#pragma once

#include <scriptos/std/types.hpp>

bool isalnum(int c);
bool isalpha(int c);
bool isblank(int c);
bool iscntrl(int c);
bool isdigit(int c);
bool isgraph(int c);
bool islower(int c);
bool isprint(int c);
bool ispunct(int c);
bool isspace(int c);
bool isupper(int c);
bool isxdigit(int c);

int uitoa(str buf, unsigned value, unsigned base, bool upper);

template <typename T, typename U>
auto ceil_div(T divisor, U dividend)
{
    return (divisor / dividend) + !!(divisor % dividend);
}

inline constexpr unsigned long long int operator""_KiB(unsigned long long int value)
{
    return value * 1024;
}

inline constexpr unsigned long long int operator""_MiB(unsigned long long int value)
{
    return value * 1024 * 1024;
}

inline constexpr unsigned long long int operator""_GiB(unsigned long long int value)
{
    return value * 1024 * 1024 * 1024;
}
