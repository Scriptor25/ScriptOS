#pragma once

#include <scriptos/types.h>

class Bitmap
{
public:
    Bitmap(u8* buffer, usize size_in_bits);

    void Clear();

    void Set(usize index, bool state);
    void Fill(usize index, usize count, bool state);
    bool Get(usize index) const;

private:
    u8* m_Buffer;
    usize m_SizeInBits;
};
