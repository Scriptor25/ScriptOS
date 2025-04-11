#pragma once

struct FILE
{
    static FILE In(int (*g)());
    static FILE Out(int (*p)(int c));

    FILE() = default;

    virtual int Put(int c);
    virtual int Get();

    int (*P)(int c) = nullptr;
    int (*G)() = nullptr;
};

template <typename T>
struct STRING : FILE
{
    STRING(T *buffer, usize length)
        : FILE(), Buffer(buffer), Length(length), Index(0)
    {
    }

    int Put(int c) override
    {
        if (Index < Length)
            return Buffer[Index++] = c;
        return -1;
    }

    int Get() override
    {
        if (Index > 0)
            return Buffer[--Index];
        return -1;
    }

    T *Buffer;
    usize Length;
    usize Index;
};

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

void InitializeStdIO();
