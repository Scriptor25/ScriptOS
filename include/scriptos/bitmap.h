#pragma once

#include <scriptos/types.h>

class Bitmap
{
    struct Entry
    {
        usize Index;
        bool State;
    };

    class Iterator
    {
    public:
        Iterator(const Bitmap& bitmap, usize index);

        Entry operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;

    private:
        const Bitmap& m_Bitmap;
        usize m_Index;
    };

public:
    Bitmap() = default;
    Bitmap(u8* buffer, usize size_in_bits);

    void Clear();

    void Set(usize index, bool state);
    void Fill(usize index, usize count, bool state);
    bool Get(usize index) const;

    Iterator begin() const;
    Iterator end() const;

private:
    u8* m_Buffer = nullptr;
    usize m_SizeInBits = 0;
};
