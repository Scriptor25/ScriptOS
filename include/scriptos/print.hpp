#pragma once

#include <scriptos/vararg.hpp>

void putchar(int c);
int print(const char *str);
int wprint(const int *str);
int printn(const char *str, int num);

int printf(const char *format, ...);
int vprintf(const char *format, va_list ap);
