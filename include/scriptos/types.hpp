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

#if defined(__i386__)
typedef int iptr;
typedef unsigned int uptr;
#elif defined(__x86_64__)
typedef long long int iptr;
typedef unsigned long long int uptr;
#else
#error "unsupported architecture"
#endif
