#pragma once

#include <scriptos/std/types.hpp>

void *memcpy(void *dst, const void *src, usize count);
void *memset(void *dst, int src, usize count);
void *memset(void *dst, int src, usize size, usize count);

int memcmp(const void *ptr1, const void *ptr2, usize count);

int strlen(cstr string);
int strlen(cwstr string);

void *malloc(usize count);
void free(void *address);

void *realloc(void *address, usize count);
void *calloc(usize count, usize size);
