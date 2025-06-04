#pragma once

#include <scriptos/types.h>
#include <scriptos/vararg.h>

int print(out_stream stream, cstr format, ...);
int vprint(out_stream stream, cstr format, va_list ap);
