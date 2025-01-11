#pragma once

#include <scriptos/types.hpp>
#include <scriptos/vararg.hpp>

void putchar(int c);
int print(cstr string);
int wprint(const int *string);
int printn(cstr string, int num);
int printf(cstr format, ...);
int vprintf(cstr format, va_list ap);
