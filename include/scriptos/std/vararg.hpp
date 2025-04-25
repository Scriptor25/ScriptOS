#pragma once

#define va_start(ap, arg) ((ap) = (va_list) (&(arg) + 1))
#define va_arg(ap, type)  (*(((type*) ((ap) = (va_list) ((uptr) (ap) + sizeof(type)))) - 1))
#define va_end(ap)        ((ap) = nullptr)

using va_list = char**;
