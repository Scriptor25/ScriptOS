#pragma once

#include <scriptos/types.hpp>

void *memcpy(void *dst, const void *src, usize count);
void *memset(void *dst, int src, usize count);
void *memset(void *dst, int src, usize size, usize count);

void *malloc(usize count);
void free(void *address);

void *realloc(void *address, usize count);
void *calloc(usize count, usize size);
