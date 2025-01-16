#include <scriptos/kernel/graphics.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

void putchar(int c)
{
    Graphics::GetInstance().PutChar(c);
}

int print(cstr string)
{
    str p;
    for (p = (str)string; *p; ++p)
        putchar(*p);
    return p - string;
}

int wprint(cwstr string)
{
    wstr p;
    for (p = (wstr)string; *p; ++p)
        putchar(*p);
    return p - string;
}

int printn(cstr string, usize num)
{
    for (usize n = 0; n < num && string[n]; ++n)
        putchar(string[n]);
    return num;
}

int wprintn(cwstr string, usize num)
{
    for (usize n = 0; n < num && string[n]; ++n)
        putchar(string[n]);
    return num;
}

int printf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto num = vprintf(format, ap);
    va_end(ap);
    return num;
}

int wprintf(cwstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto num = wvprintf(format, ap);
    va_end(ap);
    return num;
}

enum FLAG
{
    flag_none = 0,
    flag_left_justify = 1,
    flag_force_sign = 2,
    flag_blank_space = 4,
    flag_prefix = 8,
    flag_pad_zero = 16,
};

typedef struct print_int
{
    va_list ap;
    int num;
} print_int_t;

static print_int_t print_int(va_list ap, int flags, int width, int precision, bool is_signed, int base, bool uppercase)
{
    (void)precision;

    char buf[256];

    int num = 0;

    auto left_justify = flags & flag_left_justify;
    auto force_sign = flags & flag_force_sign;
    auto blank_space = flags & flag_blank_space;
    auto prefix = flags & flag_prefix;
    auto pad_zero = flags & flag_pad_zero;

    auto i = va_arg(ap, int);

    auto has_sign = is_signed && i < 0;
    if (has_sign)
        i = -i;

    auto len = uitoa(buf, (unsigned int)i, base, uppercase);

    if (!left_justify && !pad_zero)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            num++;
        }

    if (prefix)
    {
        switch (base)
        {
        case 8:
            putchar('0');
            num++;
            break;
        case 16:
            if (uppercase)
                num += print("0X");
            else
                num += print("0x");
            break;
        }
    }

    if (pad_zero)
        for (int x = len; x < width; ++x)
        {
            putchar('0');
            num++;
        }

    if (has_sign)
    {
        putchar('-');
        num++;
    }
    else if (is_signed && force_sign)
    {
        putchar('+');
        num++;
    }
    else if (blank_space)
    {
        putchar(' ');
        num++;
    }

    num += printn(buf, len);

    if (left_justify)
        for (int x = len; x < width; ++x)
        {
            putchar(' ');
            num++;
        }

    return {ap, num};
}

template <typename T>
static int tvprintf(const T *format, va_list ap)
{
    int num = 0;

    enum STATE
    {
        state_none,
        state_flags,
        state_width,
        state_precision,
        state_specifier,
    } state = state_none;

    int flags = 0, width = 0, precision = 0;

    auto p = (T *)format;

    while (*p)
    {
        switch (state)
        {
        case state_none:
            if (*p != '%')
            {
                putchar(*p++);
                num++;
                break;
            }
            state = state_flags;
            flags = flag_none;
            p++;
            break;
        case state_flags:
            switch (*p)
            {
            case '-':
                flags |= flag_left_justify;
                p++;
                break;
            case '+':
                flags |= flag_force_sign;
                p++;
                break;
            case ' ':
                flags |= flag_blank_space;
                p++;
                break;
            case '#':
                flags |= flag_prefix;
                p++;
                break;
            case '0':
                flags |= flag_pad_zero;
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
                num += result.num;
                break;
            }
            case 'u':
            case 'U':
            {
                auto result = print_int(ap, flags, width, precision, false, 10, false);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'o':
            case 'O':
            {
                auto result = print_int(ap, flags, width, precision, false, 8, false);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'x':
            {
                auto result = print_int(ap, flags, width, precision, false, 16, false);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'X':
            {
                auto result = print_int(ap, flags, width, precision, false, 16, true);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'b':
            case 'B':
            {
                auto result = print_int(ap, flags, width, precision, false, 2, false);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'c':
            case 'C':
                putchar(va_arg(ap, int));
                num++;
                break;
            case 's':
            case 'S':
                if (precision < 0)
                    num += print(va_arg(ap, cstr));
                else
                    num += printn(va_arg(ap, cstr), precision);
                break;
            case 'w':
            case 'W':
                if (precision < 0)
                    num += wprint(va_arg(ap, cwstr));
                else
                    num += wprintn(va_arg(ap, cwstr), precision);
                break;
            case 'p':
            case 'P':
            {
                char buf[16];
                auto ptr = va_arg(ap, void *);
                auto len = uitoa(buf, (uptr)ptr, 16, 0);
                for (unsigned x = len; x < sizeof(void *) * 2; ++x)
                    putchar('0');
                printn(buf, len);

                num += 16;
                break;
            }
            case '%':
                putchar('%');
                num++;
                break;
            default:
                break;
            }
            state = state_none;
            p++;
            break;
        }
    }

    return num;
}

int vprintf(cstr format, va_list ap)
{
    return tvprintf(format, ap);
}

int wvprintf(cwstr format, va_list ap)
{
    return tvprintf(format, ap);
}
