#include <scriptos/std/memory.hpp>
#include <scriptos/std/util.hpp>

bool isalnum(int c)
{
    return (0x30 <= c && c <= 0x39) || (0x41 <= c && c <= 0x5a) || (0x61 <= c && c <= 0x7a);
}

bool isalpha(int c)
{
    return (0x41 <= c && c <= 0x5a) || (0x61 <= c && c <= 0x7a);
}

bool isblank(int c)
{
    return (c == 0x09) || (c == 0x20);
}

bool iscntrl(int c)
{
    return (0x00 <= c && c <= 0x1f) || (c == 0x7f);
}

bool isdigit(int c)
{
    return (0x30 <= c && c <= 0x39);
}

bool isgraph(int c)
{
    return (0x21 <= c && c <= 0x7e);
}

bool islower(int c)
{
    return (0x61 <= c && c <= 0x7a);
}

bool isprint(int c)
{
    return (0x20 <= c && c <= 0x7e);
}

bool ispunct(int c)
{
    return (0x21 <= c && c <= 0x2f) || (0x3a <= c && c <= 0x40) || (0x5b <= c && c <= 0x60) || (0x7b <= c && c <= 0x7e);
}

bool isspace(int c)
{
    return (0x09 <= c && c <= 0x0d) || (c == 0x20);
}

bool isupper(int c)
{
    return (0x41 <= c && c <= 0x5a);
}

bool isxdigit(int c)
{
    return (0x30 <= c && c <= 0x39) || (0x41 <= c && c <= 0x46) || (0x61 <= c && c <= 0x66);
}

int uitoa(str buf, unsigned value, unsigned base, unsigned min_len, bool upper)
{
    char rev[256];

    unsigned len = 0;
    while ((value != 0) || (len < min_len))
    {
        auto rem = value % base;
        value /= base;
        rev[len++] = ((rem < 10) ? (rem + 0x30) : (rem + (upper ? 'A' : 'a') - 10));
    }

    if (buf)
        for (int i = 0, j = static_cast<int>(len) - 1; j >= 0; ++i, --j)
            buf[i] = rev[j];

    return len;
}

int fftoa(str buf, double value, unsigned precision)
{
    int len = 0;
    for (unsigned i = 0, m = 10; i < precision; ++i, m *= 10)
    {
        auto ivalue = static_cast<unsigned>(value * m);
        auto rem = ivalue % 10;
        buf[len++] = rem + 0x30;
    }

    return len;
}

f32 floor(f32 x)
{
    u32 input;
    memcpy(&input, &x, 4);

    auto exponent = static_cast<i32>((input >> 23) & 255) - 127;
    if (exponent < 0)
        return x < 0.f ? -1.f : 0.f;

    auto fractional_bits = 23 - exponent;
    if (fractional_bits <= 0)
        return x;

    auto integral_mask = ~((1U << fractional_bits) - 1);
    auto output = input & integral_mask;

    memcpy(&x, &output, 4);
    if (x < 0.f && output != input)
        --x;

    return x;
}

f64 floor(f64 x)
{
    u64 input;
    memcpy(&input, &x, 8);

    auto exponent = static_cast<i32>((input >> 52) & 0x7FF) - 1023;
    if (exponent < 0)
        return x < 0. ? -1. : 0.;

    auto fractional_bits = 52 - exponent;
    if (fractional_bits <= 0)
        return x;

    auto integral_mask = ~((1ULL << fractional_bits) - 1);
    auto output = input & integral_mask;

    memcpy(&x, &output, 8);
    if (x < 0. && output != input)
        --x;

    return x;
}

f32 ceil(f32 x)
{
    u32 input;
    memcpy(&input, &x, 4);

    auto exponent = static_cast<i32>((input >> 23) & 255) - 127;
    if (exponent < 0)
        return x > 0.f ? 1.f : 0.f;

    auto fractional_bits = 23 - exponent;
    if (fractional_bits <= 0)
        return x;

    auto integral_mask = ~((1U << fractional_bits) - 1);
    auto output = input & integral_mask;

    memcpy(&x, &output, 4);
    if (x > 0.f && output != input)
        ++x;

    return x;
}

f64 ceil(f64 x)
{
    u64 input;
    memcpy(&input, &x, 8);

    auto exponent = static_cast<i32>((input >> 52) & 0x7FF) - 1023;
    if (exponent < 0)
        return x > 0. ? 1. : 0.;

    auto fractional_bits = 52 - exponent;
    if (fractional_bits <= 0)
        return x;

    auto integral_mask = ~((1ULL << fractional_bits) - 1);
    auto output = input & integral_mask;

    memcpy(&x, &output, 8);
    if (x > 0. && output != input)
        ++x;

    return x;
}
