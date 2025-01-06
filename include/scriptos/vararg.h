#pragma once

#define NULL ((void *)0)

#define va_start(ap, arg) ((ap) = (va_list)(&(arg) + 1))
#define va_arg(ap, type) (*((type *)(ap)++))
#define va_end(ap) ((ap) = NULL)

typedef char **va_list;
