#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/serial.hpp>
#include <scriptos/std/stream.hpp>

FILE *stdin;
FILE *stdout;
FILE *stderr;

static FILE internal_stdin;
static FILE internal_stdout;
static FILE internal_stderr;

void InitializeStdIO()
{
    internal_stdin = {.Get = []()
                      { return -1; }};
    internal_stdout = {.Put = [](int c)
                       {
                           Graphics::GetKernelInstance().PutChar(c);
                           return c;
                       }};
    internal_stderr = {.Put = [](int c)
                       { Serial::Write(c); return c; }};

    stdin = &internal_stdin;
    stdout = &internal_stdout;
    stderr = &internal_stderr;
}
