#pragma once

#include <scriptos/types.h>

#define NULL ((void *)0)

void *memcpy(void *dst, const void *src, u64 num);
void *memset(void *dst, u64 src, u64 num);

void *malloc(u64 size);
void *calloc(u64 num, u64 size);
void *realloc(void *block, u64 size);
void *free(void *block);
