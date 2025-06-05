#pragma once

#include <scriptos/types.h>
#include <scriptos/vararg.h>

unsigned Print(out_stream stream, cstr format, ...);
unsigned PrintV(out_stream stream, cstr format, va_list ap);
