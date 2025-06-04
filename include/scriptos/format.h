#pragma once

#include <scriptos/types.h>
#include <scriptos/vararg.h>

unsigned print(out_stream stream, cstr format, ...);
unsigned vprint(out_stream stream, cstr format, va_list ap);
