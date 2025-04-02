#pragma once

/* cannot explicify casts */
#define va_start(ap, arg) ((ap) = (va_list)(&(arg) + 1))
#define va_arg(ap, type) (*((type *)(ap)++))
#define va_end(ap) ((ap) = nullptr)

using va_list = char **;
