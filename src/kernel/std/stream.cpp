#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/serial.hpp>
#include <scriptos/std/stream.hpp>

FILE *stdin;
FILE *stdout;
FILE *stderr;

static FILE internal_stdin;
static FILE internal_stdout;
static FILE internal_stderr;

FILE FILE::In(int (*g)())
{
    FILE stream;
    stream.G = g;
    return stream;
}

FILE FILE::Out(int (*p)(int c))
{
    FILE stream;
    stream.P = p;
    return stream;
}

int FILE::Put(int c)
{
    if (P)
        return P(c);
    return -1;
}

int FILE::Get()
{
    if (G)
        return G();
    return -1;
}

void InitializeStdIO()
{
    internal_stdin = FILE::In([]()
                              { return -1; });

    internal_stdout = FILE::Out(
        [](int c)
        {
            Graphics::GetKernelInstance().PutChar(c);
            return c;
        });

    internal_stderr = FILE::Out(
        [](int c)
        {
            Serial::Write(c);
            return c;
        });

    stdin = &internal_stdin;
    stdout = &internal_stdout;
    stderr = &internal_stderr;
}
