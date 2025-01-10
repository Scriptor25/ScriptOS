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
        explicit Iter(const Bitmap &bitmap, u32 index);

        BitEntry operator*() const;
        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        const Bitmap &m_Bitmap;
        u32 m_Index;
    };

    void
    Init(u32 size, u8 *buffer);

    u32 GetSize() const;
    void *GetBuffer() const;

    bool operator[](u32 index) const;
    bool Get(u32 index) const;
    void Set(u32 index, bool value);

    Iter begin() const;
    Iter end() const;

private:
    u32 m_Size;
    u8 *m_Buffer;
};
