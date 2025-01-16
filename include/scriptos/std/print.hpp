#pragma once

#include <scriptos/std/types.hpp>
#include <scriptos/std/vararg.hpp>

void putchar(int c);
int print(cstr string);
int wprint(cwstr string);
int printn(cstr string, usize num);
int wprintn(cwstr string, usize num);
int printf(cstr format, ...);
int wprintf(cwstr format, ...);
int vprintf(cstr format, va_list ap);
int wvprintf(cwstr format, va_list ap);
