#pragma once

#define va_start(ap, arg) ((ap) = reinterpret_cast<va_list>(&(arg) + 1))
#define va_arg(ap, type) (*((type *)(ap)++)) /* cannot explicify this cast */
#define va_end(ap) ((ap) = nullptr)

typedef char **va_list;
