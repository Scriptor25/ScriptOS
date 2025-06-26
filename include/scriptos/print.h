#pragma once

#include <scriptos/types.h>
#include <scriptos/vararg.h>

unsigned Print(
    cstr format,
    ...);
unsigned PrintV(
    cstr format,
    va_list ap);

void Flush();

unsigned SPrint(
    out_stream stream,
    cstr format,
    ...);
unsigned SPrintV(
    out_stream stream,
    cstr format,
    va_list ap);
