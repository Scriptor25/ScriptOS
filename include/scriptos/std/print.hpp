#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/vararg.hpp>

void putchar(int c);
int print(cstr string);
int wprint(cwstr string);
int printn(cstr string, usize count);
int wprintn(cwstr string, usize count);
int printf(cstr format, ...);
int wprintf(cwstr format, ...);
int vprintf(cstr format, va_list ap);
int wvprintf(cwstr format, va_list ap);
