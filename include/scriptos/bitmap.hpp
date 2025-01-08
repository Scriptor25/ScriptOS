#pragma once

#include <scriptos/types.hpp>

struct BitEntry
{
    u64 ByteIndex;
    u64 BitIndex;

    bool Active;
};

class Bitmap
{
public:
    class Iter
    {
    public:
        explicit Iter(const Bitmap &bitmap, u64 index);

        BitEntry operator*() const;
        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        const Bitmap &m_Bitmap;
        u64 m_Index;
    };

    void
    Init(u64 size, u8 *buffer);

    u64 GetSize() const;
    void *GetBuffer() const;

    bool operator[](u64 index) const;
    bool Get(u64 index) const;
    void Set(u64 index, bool value);

    Iter begin() const;
    Iter end() const;

private:
    u64 m_Size;
    u8 *m_Buffer;
};
