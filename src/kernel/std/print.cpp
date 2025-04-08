#include <scriptos/kernel/graphics.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

void putchar(int c)
{
    Graphics::GetKernelInstance().PutChar(c);
}

int print(cstr string)
{
    if (!string)
        return print("(null)");

    str p;
    for (p = const_cast<str>(string); *p; ++p)
        putchar(*p);
    return p - string;
}

int wprint(cwstr string)
{
    if (!string)
        return wprint(L"(null)");

    wstr p;
    for (p = const_cast<wstr>(string); *p; ++p)
        putchar(*p);
    return p - string;
}

int printn(cstr string, usize count)
{
    if (!string)
        return print("(null)");

    for (usize n = 0; n < count && string[n]; ++n)
        putchar(string[n]);
    return count;
}

int wprintn(cwstr string, usize count)
{
    if (!string)
        return wprint(L"(null)");

    for (usize n = 0; n < count && string[n]; ++n)
        putchar(string[n]);
    return count;
}

int printf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = vprintf(format, ap);
    va_end(ap);
    return count;
}

int wprintf(cwstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = wvprintf(format, ap);
    va_end(ap);
    return count;
}

enum FLAG
{
    FLAG_NONE = 0 << 0,
    FLAG_LEFT_JUSTIFY = 1 << 0,
    FLAG_FORCE_SIGN = 1 << 1,
    FLAG_BLANK_SPACE = 1 << 2,
    FLAG_PREFIX = 1 << 3,
    FLAG_PAD_ZERO = 1 << 4,
};

struct print_result
{
    va_list ap;
    int count;
};

static print_result print_int(va_list ap, int flags, int width, int precision, bool is_signed, int base, bool uppercase)
{
    char buf[256];

    int count = 0;

    auto left_justify = flags & FLAG_LEFT_JUSTIFY;
    auto force_sign = flags & FLAG_FORCE_SIGN;
    auto blank_space = flags & FLAG_BLANK_SPACE;
    auto prefix = flags & FLAG_PREFIX;
    auto pad_zero = flags & FLAG_PAD_ZERO;

    auto value = va_arg(ap, int);

    auto has_sign = is_signed && value < 0;
    if (has_sign)
        value = -value;

    auto len = uitoa(buf, static_cast<unsigned int>(value), base, precision < 0 ? 1 : precision, uppercase);

    if (!left_justify && !pad_zero)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            count++;
        }

    if (prefix)
    {
        switch (base)
        {
        case 2:
            if (uppercase)
                count += print("0B");
            else
                count += print("0b");
            break;
        case 8:
            putchar('0');
            count++;
            break;
        case 16:
            if (uppercase)
                count += print("0X");
            else
                count += print("0x");
            break;
        }
    }

    if (pad_zero)
        for (int x = len; x < width; ++x)
        {
            putchar('0');
            count++;
        }

    if (has_sign)
    {
        putchar('-');
        count++;
    }
    else if (is_signed && force_sign)
    {
        putchar('+');
        count++;
    }
    else if (blank_space)
    {
        putchar(' ');
        count++;
    }

    count += printn(buf, len);

    if (left_justify)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            count++;
        }

    return {ap, count};
}

/**
 * print a formatted floating point number
 *
 * - ap: arguments pointer
 * - flags: formatting flags
 * - width: minimum number of characters to be printed
 * - precision: number of floating point digits
 * - info:
 *   [0]: is uppercase
 *   [1]: scientific
 *   [2]: shortest
 *   [3]: hexadecimal
 */
static print_result print_float(va_list ap, int flags, int width, int precision, int info)
{
    (void)width; // TODO: minimum width
    (void)info;  // TODO: scientific or hexadecimal version

    char int_buf[256];
    char flt_buf[256];

    auto left_justify = flags & FLAG_LEFT_JUSTIFY;
    auto force_sign = flags & FLAG_FORCE_SIGN;
    auto blank_space = flags & FLAG_BLANK_SPACE;
    auto prefix = flags & FLAG_PREFIX;
    auto pad_zero = flags & FLAG_PAD_ZERO;

    (void)left_justify; // TODO: justify left
    (void)force_sign;   // TODO: always preceed with sign
    (void)blank_space;  // TODO: space when no sign
    (void)pad_zero;     // TODO: left pad with leading zeros

    int count = 0;
    auto value = va_arg(ap, double);

    auto int_len = uitoa(int_buf, static_cast<unsigned int>(value), 10, 1, false);
    auto flt_len = fftoa(flt_buf, value, precision < 0 ? 6 : precision);

    count += printn(int_buf, int_len);
    if (prefix || flt_len != 0)
    {
        putchar('.');
        count += 1;
        count += printn(flt_buf, flt_len);
    }

    return {ap, count};
}

static print_result print_string(va_list ap, int flags, int width, int precision)
{
    int count = 0;

    auto left_justify = flags & FLAG_LEFT_JUSTIFY;

    auto value = va_arg(ap, cstr);

    auto len = (precision < 0) ? strlen(value) : min(precision, strlen(value));

    if (!left_justify)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            count++;
        }

    count += printn(value, len);

    if (left_justify)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            count++;
        }

    return {ap, count};
}

static print_result print_wstring(va_list ap, int flags, int width, int precision)
{
    int count = 0;

    auto left_justify = flags & FLAG_LEFT_JUSTIFY;

    auto value = va_arg(ap, cwstr);

    auto len = (precision < 0) ? strlen(value) : min(precision, strlen(value));

    if (!left_justify)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            count++;
        }

    count += wprintn(value, len);

    if (left_justify)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            count++;
        }

    return {ap, count};
}

template <typename T>
static int tvprintf(const T *format, va_list ap)
{
    int count = 0;

    enum STATE
    {
        state_none,
        state_flags,
        state_width,
        state_precision,
        state_specifier,
    } state = state_none;

    int flags = 0, width = 0, precision = 0;

    auto p = const_cast<T *>(format);

    while (*p)
    {
        switch (state)
        {
        case state_none:
            if (*p != '%')
            {
                putchar(*p++);
                count++;
                break;
            }
            state = state_flags;
            flags = FLAG_NONE;
            p++;
            break;
        case state_flags:
            switch (*p)
            {
            case '-':
                flags |= FLAG_LEFT_JUSTIFY;
                p++;
                break;
            case '+':
                flags |= FLAG_FORCE_SIGN;
                p++;
                break;
            case ' ':
                flags |= FLAG_BLANK_SPACE;
                p++;
                break;
            case '#':
                flags |= FLAG_PREFIX;
                p++;
                break;
            case '0':
                flags |= FLAG_PAD_ZERO;
                p++;
                break;
            default:
                state = state_width;
                width = -1;
                break;
            }
            break;
        case state_width:
            if (width < 0)
            {
                if (*p == '*')
                {
                    width = va_arg(ap, unsigned int);
                    state = state_precision;
                    precision = -1;
                    p++;
                    break;
                }
                width = 0;
            }
            if (isdigit(*p))
            {
                width = width * 10 + (*p - 0x30);
                p++;
                break;
            }
            state = state_precision;
            precision = -1;
            break;
        case state_precision:
            if (precision < 0)
            {
                if (*p != '.')
                {
                    state = state_specifier;
                    break;
                }
                precision = 0;
                p++;
                break;
            }
            if (precision == 0)
            {
                if (*p == '*')
                {
                    precision = va_arg(ap, unsigned int);
                    state = state_specifier;
                    p++;
                    break;
                }
            }
            if (isdigit(*p))
            {
                precision = precision * 10 + (*p - 0x30);
                p++;
                break;
            }
            state = state_specifier;
            break;
        case state_specifier:
            switch (*p)
            {
            case 'd':
            case 'D':
            case 'i':
            case 'I':
            {
                auto result = print_int(ap, flags, width, precision, true, 10, false);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'u':
            case 'U':
            {
                auto result = print_int(ap, flags, width, precision, false, 10, false);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'o':
            case 'O':
            {
                auto result = print_int(ap, flags, width, precision, false, 8, false);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'x':
            {
                auto result = print_int(ap, flags, width, precision, false, 16, false);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'X':
            {
                auto result = print_int(ap, flags, width, precision, false, 16, true);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'b':
            case 'B':
            {
                auto result = print_int(ap, flags, width, precision, false, 2, false);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'f':
            case 'F':
            {
                auto result = print_float(ap, flags, width, precision, 0b0000);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'e':
            {
                auto result = print_float(ap, flags, width, precision, 0b0010);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'E':
            {
                auto result = print_float(ap, flags, width, precision, 0b0011);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'g':
            {
                auto result = print_float(ap, flags, width, precision, 0b0100);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'G':
            {
                auto result = print_float(ap, flags, width, precision, 0b0101);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'a':
            {
                auto result = print_float(ap, flags, width, precision, 0b1000);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'A':
            {
                auto result = print_float(ap, flags, width, precision, 0b1001);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'c':
            case 'C':
            {
                putchar(va_arg(ap, int));
                count++;
                break;
            }
            case 's':
            case 'S':
            {
                auto result = print_string(ap, flags, width, precision);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'w':
            case 'W':
            {
                auto result = print_wstring(ap, flags, width, precision);
                ap = result.ap;
                count += result.count;
                break;
            }
            case 'p':
            case 'P':
            {
                char buf[8];
                auto ptr = va_arg(ap, void *);
                auto len = uitoa(buf, reinterpret_cast<uptr>(ptr), 16, 8, false);
                count += printn(buf, len);
                break;
            }
            case 'n':
            {
                auto ptr = va_arg(ap, int *);
                *ptr = count;
                break;
            }
            case '%':
            {
                putchar('%');
                count++;
                break;
            }
            default:
                break;
            }
            state = state_none;
            p++;
            break;
        }
    }

    return count;
}

int vprintf(cstr format, va_list ap)
{
    return tvprintf(format, ap);
}

int wvprintf(cwstr format, va_list ap)
{
    return tvprintf(format, ap);
}
