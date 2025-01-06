#include <scriptos/print.h>
#include <scriptos/types.h>
#include <scriptos/util.h>

int print(const char *str)
{
    char *p;
    for (p = (char *)str; *p; ++p)
        putchar(*p);
    return p - str;
}

int printn(const char *str, int num)
{
    for (int n = 0; n < num; ++n)
        putchar(str[n]);
    return num;
}

int printf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int num = vprintf(format, ap);
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

static print_int_t print_int(va_list ap, int flags, int width, int precision, int is_signed, int base, int uppercase)
{
    (void)precision;

    char buf[256];

    int num = 0;

    int left_justify = flags & flag_left_justify;
    int force_sign = flags & flag_force_sign;
    int blank_space = flags & flag_blank_space;
    int prefix = flags & flag_prefix;
    int pad_zero = flags & flag_pad_zero;

    int i = va_arg(ap, int);

    int has_sign = is_signed && i < 0;
    if (has_sign)
        i = -i;

    int len = itoa(buf, (unsigned int)i, base, uppercase);

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

    return (print_int_t){ap, num};
}

int vprintf(const char *format, va_list ap)
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

    char *p = (char *)format;

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
            if (is_digit(*p))
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
            if (is_digit(*p))
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
                print_int_t result = print_int(ap, flags, width, precision, 1, 10, 0);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'u':
            case 'U':
            {
                print_int_t result = print_int(ap, flags, width, precision, 0, 10, 0);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'o':
            case 'O':
            {
                print_int_t result = print_int(ap, flags, width, precision, 0, 8, 0);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'x':
            {
                print_int_t result = print_int(ap, flags, width, precision, 0, 16, 0);
                ap = result.ap;
                num += result.num;
                break;
            }
            case 'X':
            {
                print_int_t result = print_int(ap, flags, width, precision, 0, 16, 1);
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
                    num += print(va_arg(ap, const char *));
                else
                    num += printn(va_arg(ap, const char *), precision);
                break;
            case 'p':
            case 'P':
            {
                unsigned int lower = va_arg(ap, unsigned int);
                char buf_lower[8];
                int len_lower = itoa(buf_lower, lower, 16, 0);

                unsigned int upper = va_arg(ap, unsigned int);
                char buf_upper[8];
                int len_upper = itoa(buf_upper, upper, 16, 0);

                for (int x = len_upper; x < 8; ++x)
                    putchar('0');
                printn(buf_upper, len_upper);

                for (int x = len_lower; x < 8; ++x)
                    putchar('0');
                printn(buf_lower, len_lower);

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
