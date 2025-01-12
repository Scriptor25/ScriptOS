#pragma once

typedef signed char i8;
typedef unsigned char u8;
typedef signed short int i16;
typedef unsigned short int u16;
typedef signed long int i32;
typedef unsigned long int u32;
typedef signed long long int i64;
typedef unsigned long long int u64;

typedef float f32;
typedef double f64;
typedef long double f128;

typedef char *str;
typedef const char *cstr;

typedef decltype(nullptr) nullptr_t;

#if defined(__i386__) || defined(__i686__)
typedef int iptr;
typedef unsigned int uptr;
typedef long int isize;
typedef unsigned long int usize;
#elif defined(__x86_64__)
typedef long long int iptr;
typedef unsigned long long int uptr;
typedef long int isize;
typedef unsigned long int usize;
#else
#error "unsupported architecture"
#endif
