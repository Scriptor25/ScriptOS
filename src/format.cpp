#include <scriptos/format.h>
#include <scriptos/types.h>

int print(out_stream stream, cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto count = vprint(stream, format, ap);
    va_end(ap);
    return count;
}

static int isdigit(int c)
{
    return '0' <= c && c <= '9';
}

enum
{
    flags_left_justify,
    flags_force_sign,
    flags_sign_space,
    flags_prefix,
    flags_pad_zero,
};

static int print_int(out_stream stream, u64 value, unsigned base, bool uppercase, bool is_signed, unsigned flags, int width, int precision)
{
    (void) precision;

    bool left_justify = flags == flags_left_justify;
    bool force_sign = flags == flags_force_sign;
    bool sign_space = flags == flags_sign_space;
    bool prefix = flags == flags_prefix;
    bool pad_zero = flags == flags_pad_zero;

    auto has_sign = is_signed && static_cast<i64>(value) < 0;

    if (has_sign)
        value = static_cast<u64>(-static_cast<i64>(value));

    char buffer[128];
    int count = 0;

    do
    {
        auto rem = value % base;
        value /= base;
        buffer[count++] = rem + (rem >= 10 ? (uppercase ? 'A' : 'a') - 10 : '0');
    } while (value);

    if (has_sign)
        buffer[count++] = '-';
    else if (force_sign)
        buffer[count++] = '+';
    else if (sign_space)
        buffer[count++] = ' ';
    else if (prefix)
        switch (base)
        {
        case 8:
            buffer[count++] = '0';
            break;

        case 16:
            buffer[count++] = (uppercase ? 'X' : 'x');
            buffer[count++] = '0';
            break;
        }

    if (!left_justify)
        while (count < width)
            buffer[count++] = (pad_zero ? '0' : ' ');

    for (int i = count - 1; i >= 0; --i)
        stream(buffer[i]);

    if (left_justify)
        while (count++ < width)
            stream(' ');

    return count;
}

int vprint(out_stream stream, cstr format, va_list ap)
{
    // %[flags][width][.precision][length]specifier

    int count = 0;

    enum
    {
        state_print,
        state_flags,
        state_width,
        state_precision,
        state_specifier,
    } state = state_print;

    unsigned flags;
    int width, precision;

    for (str ptr = const_cast<str>(format); *ptr;)
    {
        switch (state)
        {
        case state_print:
            if (*ptr == '%')
            {
                ptr++;
                state = state_flags;
                flags = 0;
                break;
            }
            stream(*ptr++);
            count++;
            break;

        case state_flags:
            switch (*ptr)
            {
            case '-':
                ptr++;
                flags = flags_left_justify;
                break;
            case '+':
                ptr++;
                flags = flags_force_sign;
                break;
            case ' ':
                ptr++;
                flags = flags_sign_space;
                break;
            case '#':
                ptr++;
                flags = flags_prefix;
                break;
            case '0':
                ptr++;
                flags = flags_pad_zero;
                break;
            }
            state = state_width;
            width = 0;
            break;

        case state_width:
            if (isdigit(*ptr))
            {
                width = width * 10 + (*ptr++ - '0');
                break;
            }
            if (*ptr == '*')
            {
                ptr++;
                width = va_arg(ap, unsigned);
                break;
            }
            precision = 0;
            if (*ptr == '.')
            {
                ptr++;
                state = state_precision;
                break;
            }
            state = state_specifier;
            break;

        case state_precision:
            if (isdigit(*ptr))
            {
                precision = precision * 10 + (*ptr++ - '0');
                break;
            }
            if (*ptr == '*')
            {
                ptr++;
                precision = va_arg(ap, unsigned);
                break;
            }
            state = state_specifier;
            break;

        case state_specifier:
            switch (*ptr++)
            {
            case 'd':
            case 'D':
            case 'i':
            case 'I':
            {
                auto value = va_arg(ap, u64);
                count += print_int(stream, value, 10, 0, 1, flags, width, precision);
                break;
            }

            case 'u':
            case 'U':
            {
                auto value = va_arg(ap, u64);
                count += print_int(stream, value, 10, 0, 0, flags, width, precision);
                break;
            }

            case 'o':
            case 'O':
            {
                auto value = va_arg(ap, u64);
                count += print_int(stream, value, 8, 0, 0, flags, width, precision);
                break;
            }

            case 'x':
            {
                auto value = va_arg(ap, u64);
                count += print_int(stream, value, 16, 0, 0, flags, width, precision);
                break;
            }

            case 'X':
            {
                auto value = va_arg(ap, u64);
                count += print_int(stream, value, 16, 1, 0, flags, width, precision);
                break;
            }

            case 'f':
            case 'F':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'e':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'E':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'g':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'G':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'a':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'A':
            {
                auto value = va_arg(ap, f64);
                (void) value;
                break;
            }

            case 'c':
            case 'C':
            {
                auto value = va_arg(ap, int);
                stream(value);
                count++;
                break;
            }

            case 's':
            case 'S':
            {
                auto value = va_arg(ap, str);
                for (; *value; ++value, ++count)
                    stream(*value);
                break;
            }

            case 'p':
            case 'P':
            {
                auto value = va_arg(ap, uptr);
                count += print_int(stream, value, 16, 0, 0, flags, width, precision);
                break;
            }

            case 'n':
            case 'N':
            {
                auto np = va_arg(ap, int*);
                *np = count;
                break;
            }

            case '%':
                stream('%');
                count++;
                break;
            }
            state = state_print;
            break;
        }
    }

    return count;
}
