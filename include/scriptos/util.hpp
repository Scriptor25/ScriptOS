#pragma once



bool is_digit(int c);

int itoa(char *buf, unsigned int value, unsigned int base, unsigned int upper);

template <typename T, typename U, typename V>
T ceil_div(U divisor, V dividend)
{
    return (divisor / dividend) + !!(divisor % dividend);
}
