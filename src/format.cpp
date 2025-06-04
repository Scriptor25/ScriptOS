#include <scriptos/format.h>
#include <scriptos/types.h>

int print(out_stream stream, cstr format, ...)
{
    arg_ptr args;
    arg_start(args, format);
    return vprint(stream, format, args);
}

static int isdigit(int c)
{
    return '0' <= c && c <= '9';
}

int vprint(out_stream stream, cstr format, arg_ptr args)
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

    enum
    {
        flags_none,
        flags_left_justify,
        flags_force_sign,
        flags_sign_space,
        flags_prefix,
        flags_pad_zero,
    } flags = flags_none;

    int width, precision;

    for (str ptr = const_cast<str>(format); *ptr;)
    {
        switch (state)
        {
        case state_print:
            if (*ptr++ == '%')
            {
                state = state_flags;
                flags = flags_none;
                break;
            }
            stream(*ptr++);
            count++;
            break;

        case state_flags:
            switch (*ptr)
            {
            case '-':
                flags = flags_left_justify;
                break;
            case '+':
                flags = flags_force_sign;
                break;
            case ' ':
                flags = flags_sign_space;
                break;
            case '#':
                flags = flags_prefix;
                break;
            case '0':
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
            state = state_specifier;
            break;

        case state_specifier:
            switch (*ptr++)
            {
            case 'd':
            case 'D':
            case 'i':
            case 'I':
                break;

            case 'u':
            case 'U':
                break;

            case 'o':
            case 'O':
                break;

            case 'x':
                break;

            case 'X':
                break;

            case 'f':
            case 'F':
                break;

            case 'e':
                break;

            case 'E':
                break;

            case 'g':
                break;

            case 'G':
                break;

            case 'a':
                break;

            case 'A':
                break;

            case 'c':
            case 'C':
                break;

            case 's':
            case 'S':
                break;

            case 'p':
            case 'P':
                break;

            case 'n':
            case 'N':
            {
                auto np = arg_get(args, int*);
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
