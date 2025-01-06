#include <scriptos/util.hpp>

int is_digit(int c)
{
    return 0x30 <= c && c <= 0x39;
}

int itoa(char *buf, unsigned int value, unsigned int base, unsigned int upper)
{
    int len = 0;
    char rev[256];

    while (value)
    {
        unsigned int rem = value % base;
        rev[len++] = ((rem < 10) ? (rem + 0x30) : (rem + (upper ? 'A' : 'a') - 10));
        value /= base;
    }

    if (buf)
        for (int i = 0, j = len - 1; j >= 0; ++i, --j)
            buf[i] = rev[j];

    return len;
}
