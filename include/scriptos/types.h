#pragma once

#ifndef TYPES_H
#define TYPES_H

typedef __INT8_TYPE__ i8;
typedef __UINT8_TYPE__ u8;
typedef __INT16_TYPE__ i16;
typedef __UINT16_TYPE__ u16;
typedef __INT32_TYPE__ i32;
typedef __UINT32_TYPE__ u32;
typedef __INT64_TYPE__ i64;
typedef __UINT64_TYPE__ u64;

typedef __INTPTR_TYPE__ iptr;
typedef __UINTPTR_TYPE__ uptr;
typedef __PTRDIFF_TYPE__ ptrdiff;
typedef __SIZE_TYPE__ usize;

typedef float f32;
typedef double f64;
typedef long double f128;

typedef char* str;
typedef const char* cstr;
typedef int* wstr;
typedef const int* cwstr;

typedef void (*out_stream)(char);

typedef char* arg_ptr;

#define arg_start(PTR, ARG) ((void) (PTR = (arg_ptr) (&ARG + 1)))
#define arg_get(PTR, TYPE)  (PTR += sizeof(TYPE), *(TYPE*) (PTR - sizeof(TYPE)))

#endif
