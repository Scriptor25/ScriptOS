#pragma once

#include <scriptos/types.hpp>

#define PAGE_SIZE 0x1000

void *memcpy(void *dst, const void *src, usize num);
void *memset(void *dst, int src, usize num);
