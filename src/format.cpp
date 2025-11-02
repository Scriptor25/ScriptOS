#include <scriptos/print.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

enum
{
    state_print,
    state_flags,
    state_width,
    state_precision,
    state_length,
    state_specifier,
};

enum
{
    flags_none,
    flags_left_justify,
    flags_force_sign,
    flags_sign_space,
    flags_prefix,
    flags_pad_zero,
};

static int print_int(
    out_stream stream,
    u64 value,
    int base,
    bool uppercase,
    bool is_signed,
    int flags,
    int width,
    int precision)
{
    (void) precision;

    auto left_justify = flags == flags_left_justify;
    auto force_sign = flags == flags_force_sign;
    auto sign_space = flags == flags_sign_space;
    auto prefix = flags == flags_prefix;
    auto pad_zero = flags == flags_pad_zero;

    auto has_sign = is_signed && static_cast<i64>(value) < 0;

    if (has_sign)
    {
        value = static_cast<u64>(-static_cast<i64>(value));
    }

    char buffer[256];
    int count = 0;

    do
    {
        auto rem = value % base;
        value /= base;
        buffer[count++] = rem + (rem >= 10 ? (uppercase ? 'A' : 'a') - 10 : '0');
    } while (value);

    if (has_sign)
    {
        buffer[count++] = '-';
    }
    else if (force_sign)
    {
        buffer[count++] = '+';
    }
    else if (sign_space)
    {
        buffer[count++] = ' ';
    }
    else if (prefix)
    {
        switch (base)
        {
        case 010:
            buffer[count++] = '0';
            break;

        case 0x10:
            buffer[count++] = (uppercase ? 'X' : 'x');
            buffer[count++] = '0';
            break;
        }
    }

    if (!left_justify)
    {
        while (count < width)
        {
            buffer[count++] = (pad_zero ? '0' : ' ');
        }
    }

    for (int i = count - 1; i >= 0; --i)
    {
        stream(buffer[i]);
    }

    if (left_justify)
    {
        while (count++ < width)
        {
            stream(' ');
        }
    }

    return count;
}

static int print_flt(
    out_stream stream,
    f64 value,
    int base,
    bool uppercase,
    bool scientific,
    int flags,
    int width,
    int precision)
{
    (void) stream;
    (void) value;
    (void) base;
    (void) uppercase;
    (void) scientific;
    (void) flags;
    (void) width;
    (void) precision;

    return 0;
}

int vfkprintf(
    out_stream stream,
    cstr format,
    va_list ap)
{
    // %[flags][width][.precision][length]specifier

    int count = 0;

    auto state = state_print;

    int flags, width, precision, length;

    while (*format)
    {
        switch (state)
        {
        case state_print:
            if (*format == '%')
            {
                format++;
                state = state_flags;
                flags = 0;
                break;
            }
            stream(*format++);
            count++;
            break;

        case state_flags:
            switch (*format)
            {
            case '-':
                format++;
                flags |= flags_left_justify;
                break;
            case '+':
                format++;
                flags |= flags_force_sign;
                break;
            case ' ':
                format++;
                flags |= flags_sign_space;
                break;
            case '#':
                format++;
                flags |= flags_prefix;
                break;
            case '0':
                format++;
                flags |= flags_pad_zero;
                break;
            }
            state = state_width;
            width = 0;
            break;

        case state_width:
            if ('0' <= *format && *format <= '9')
            {
                width = width * 10 + (*format++ - '0');
                break;
            }
            if (*format == '*')
            {
                format++;
                width = va_arg(ap, unsigned);
                break;
            }
            precision = 0;
            if (*format == '.')
            {
                format++;
                state = state_precision;
                break;
            }
            state = state_length;
            length = 0;
            break;

        case state_precision:
            if ('0' <= *format && *format <= '9')
            {
                precision = precision * 10 + (*format++ - '0');
                break;
            }
            if (*format == '*')
            {
                format++;
                precision = va_arg(ap, unsigned);
                break;
            }
            state = state_length;
            length = 0;
            break;

        case state_length:
            if (*format == 'l')
            {
                format++;
                length++;
                break;
            }
            state = state_specifier;
            break;

        case state_specifier:
            switch (*format++)
            {
            case 'd':
            case 'i':
            {
                long long int value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, long int);
                    break;
                case 2:
                    value = va_arg(ap, long long int);
                    break;
                default:
                    value = va_arg(ap, int);
                    break;
                }
                count += print_int(stream, value, 10, false, true, flags, width, precision);
                break;
            }

            case 'u':
            {
                unsigned long long int value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, unsigned long int);
                    break;
                case 2:
                    value = va_arg(ap, unsigned long long int);
                    break;
                default:
                    value = va_arg(ap, unsigned int);
                    break;
                }
                count += print_int(stream, value, 10, false, false, flags, width, precision);
                break;
            }

            case 'o':
            {
                unsigned long long int value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, unsigned long int);
                    break;
                case 2:
                    value = va_arg(ap, unsigned long long int);
                    break;
                default:
                    value = va_arg(ap, unsigned int);
                    break;
                }
                count += print_int(stream, value, 010, false, false, flags, width, precision);
                break;
            }

            case 'x':
            {
                unsigned long long int value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, unsigned long int);
                    break;
                case 2:
                    value = va_arg(ap, unsigned long long int);
                    break;
                default:
                    value = va_arg(ap, unsigned int);
                    break;
                }
                count += print_int(stream, value, 0x10, false, false, flags, width, precision);
                break;
            }

            case 'X':
            {
                unsigned long long int value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, unsigned long int);
                    break;
                case 2:
                    value = va_arg(ap, unsigned long long int);
                    break;
                default:
                    value = va_arg(ap, unsigned int);
                    break;
                }
                count += print_int(stream, value, 0x10, true, false, flags, width, precision);
                break;
            }

            case 'f':
            {
                long double value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, long double);
                    break;
                default:
                    value = va_arg(ap, double);
                    break;
                }
                count += print_flt(stream, value, 10, false, false, flags, width, precision);
                break;
            }

            case 'e':
            {
                long double value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, long double);
                    break;
                default:
                    value = va_arg(ap, double);
                    break;
                }
                count += print_flt(stream, value, 10, false, true, flags, width, precision);
                break;
            }

            case 'E':
            {
                long double value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, long double);
                    break;
                default:
                    value = va_arg(ap, double);
                    break;
                }
                count += print_flt(stream, value, 10, true, true, flags, width, precision);
                break;
            }

            case 'a':
            {
                long double value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, long double);
                    break;
                default:
                    value = va_arg(ap, double);
                    break;
                }
                count += print_flt(stream, value, 0x10, false, false, flags, width, precision);
                break;
            }

            case 'A':
            {
                long double value;
                switch (length)
                {
                case 1:
                    value = va_arg(ap, long double);
                    break;
                default:
                    value = va_arg(ap, double);
                    break;
                }
                count += print_flt(stream, value, 0x10, true, false, flags, width, precision);
                break;
            }

            case 'c':
            {
                auto value = va_arg(ap, int);
                stream(value);
                count++;
                break;
            }

            case 's':
            {
                auto value = va_arg(ap, char*);
                if (!value)
                {
                    stream('(');
                    stream('n');
                    stream('u');
                    stream('l');
                    stream('l');
                    stream(')');
                    count += 6;
                    break;
                }

                for (int n = 0; *value && (!precision || n < precision); ++value, ++count, ++n)
                {
                    stream(*value);
                }
                break;
            }

            case 'h':
            {
                auto value = va_arg(ap, short*);
                if (!value)
                {
                    stream('(');
                    stream('n');
                    stream('u');
                    stream('l');
                    stream('l');
                    stream(')');
                    count += 6;
                    break;
                }

                for (int n = 0; *value && (!precision || n < precision); ++value, ++count, ++n)
                {
                    stream(*value);
                }
                break;
            }

            case 'w':
            {
                auto value = va_arg(ap, int*);
                if (!value)
                {
                    stream('(');
                    stream('n');
                    stream('u');
                    stream('l');
                    stream('l');
                    stream(')');
                    count += 6;
                    break;
                }

                for (int n = 0; *value && (!precision || n < precision); ++value, ++count, ++n)
                {
                    stream(*value);
                }
                break;
            }

            case 'p':
            {
                auto value = va_arg(ap, void*);
                count += print_int(stream, reinterpret_cast<uptr>(value), 0x10, 0, 0, flags, width, precision);
                break;
            }

            case 'n':
            {
                if (auto p = va_arg(ap, int*))
                {
                    *p = count;
                }
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
