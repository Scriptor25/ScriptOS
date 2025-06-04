#pragma once

#include <scriptos/types.h>

int print(out_stream stream, cstr format, ...);
int vprint(out_stream stream, cstr format, arg_ptr args);
