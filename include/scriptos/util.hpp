#pragma once

#include <scriptos/types.hpp>

#define KiB(BYTES) (BYTES / 1024)
#define MiB(BYTES) (BYTES / (1024 * 1024))
#define GiB(BYTES) (BYTES / (1024 * 1024 * 1024))
#define TiB(BYTES) (BYTES / (1024 * 1024 * 1024 * 1024))

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
constexpr auto ceil_div(T divisor, U dividend)
{
    return (divisor / dividend) + !!(divisor % dividend);
}
