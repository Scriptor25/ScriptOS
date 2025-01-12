#pragma once

#include <scriptos/types.hpp>

struct BitEntry
{
    u32 ByteIndex;
    u32 BitIndex;

    bool Active;
};

class Bitmap
{
public:
    class Iter
    {
    public:
        explicit Iter(const Bitmap &bitmap, usize index);

        BitEntry operator*() const;
        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        const Bitmap &m_Bitmap;
        usize m_Index;
    };

    void
    Init(usize size, u8 *buffer);

    usize Size() const;
    void *Buffer() const;

    bool operator[](usize index) const;
    bool Get(usize index) const;
    bool Set(usize index);
    bool Clr(usize index);

    Iter begin() const;
    Iter end() const;

private:
    usize m_Size;
    u8 *m_Buffer;
};
