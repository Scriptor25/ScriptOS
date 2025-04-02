#pragma once

#include <scriptos/std/types.hpp>

#define NORET __attribute__((noreturn))
#define INTER __attribute__((interrupt))

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

f32 floor(f32 x);
f64 floor(f64 x);

f32 ceil(f32 x);
f64 ceil(f64 x);

template <typename V, typename T>
V lerp(V a, V b, T t)
{
    return (1 - t) * a + t * b;
}

template <typename T, typename U>
auto ceil_div(T divisor, U dividend)
{
    return (divisor / dividend) + !!(divisor % dividend);
}

template <typename T, typename U>
auto min(T lhs, U rhs)
{
    return lhs < rhs ? lhs : rhs;
}

template <typename T, typename U>
auto max(T lhs, U rhs)
{
    return lhs > rhs ? lhs : rhs;
}

template <typename T, typename U, typename V>
auto clamp(T min, U max, V x)
{
    return ::max(::min(x, max), min);
}

inline constexpr unsigned long long int operator""_KiB(unsigned long long int bytes)
{
    return bytes * 1024;
}

inline constexpr unsigned long long int operator""_MiB(unsigned long long int bytes)
{
    return bytes * 1024 * 1024;
}

inline constexpr unsigned long long int operator""_GiB(unsigned long long int bytes)
{
    return bytes * 1024 * 1024 * 1024;
}

inline constexpr unsigned long long int operator""_TiB(unsigned long long int bytes)
{
    return bytes * 1024 * 1024 * 1024 * 1024;
}

inline constexpr unsigned long long int operator""_KB(unsigned long long int bytes)
{
    return bytes * 1000;
}

inline constexpr unsigned long long int operator""_MB(unsigned long long int bytes)
{
    return bytes * 1000 * 1000;
}

inline constexpr unsigned long long int operator""_GB(unsigned long long int bytes)
{
    return bytes * 1000 * 1000 * 1000;
}

inline constexpr unsigned long long int operator""_TB(unsigned long long int bytes)
{
    return bytes * 1000 * 1000 * 1000 * 1000;
}
