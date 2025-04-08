#pragma once

#include <scriptos/std/stream.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/vararg.hpp>

int fputc(int c, FILE *stream);
int fputs(cstr s, FILE *stream);
int fputs(cwstr s, FILE *stream);
int fputn(cstr s, usize count, FILE *stream);
int fputn(cwstr s, usize count, FILE *stream);
int fprintf(FILE *stream, cstr format, ...);
int fprintf(FILE *stream, cwstr format, ...);
int vfprintf(FILE *stream, cstr format, va_list ap);
int vfprintf(FILE *stream, cwstr format, va_list ap);

int putc(int c);
int puts(cstr s);
int puts(cwstr s);
int putn(cstr s, usize count);
int putn(cwstr s, usize count);
int printf(cstr format, ...);
int printf(cwstr format, ...);
int vprintf(cstr format, va_list ap);
int vprintf(cwstr format, va_list ap);
