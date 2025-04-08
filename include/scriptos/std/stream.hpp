#pragma once

struct FILE
{
    int (*Put)(int c) = nullptr;
    int (*Get)() = nullptr;
};

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

void InitializeStdIO();
