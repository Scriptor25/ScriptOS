#pragma once

#include <scriptos/types.h>
#include <scriptos/vararg.h>

void kstdout(int c);
void kflush();

void kputc(int c);
void kputs(cstr s);

void fkputc(
    out_stream stream,
    int c);
void fkputs(
    out_stream stream,
    cstr s);

int kprintf(
    cstr format,
    ...);
int vkprintf(
    cstr format,
    va_list ap);

int fkprintf(
    out_stream stream,
    cstr format,
    ...);
int vfkprintf(
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
