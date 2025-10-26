#pragma once

#include <scriptos/types.h>
#include <scriptos/vararg.h>

unsigned kprintf(
    cstr format,
    ...);
unsigned vkprintf(
    cstr format,
    va_list ap);

void kflush();

unsigned fkprintf(
    out_stream stream,
    cstr format,
    ...);
unsigned vfkprintf(
    out_stream stream,
    cstr format,
    va_list ap);

void kprintmem(
    const void* buffer,
    usize buffer_length);
void fkprintmem(
    out_stream stream,
    const void* buffer,
    usize buffer_length);
