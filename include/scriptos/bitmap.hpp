#pragma once

#include <scriptos/types.hpp>

class Bitmap
{
public:
    void Init(u64 size, u8 *buffer);

    u64 GetSize() const;
    void *GetBuffer() const;

    bool operator[](u64 index) const;
    bool Get(u64 index) const;
    void Put(u64 index, bool value);

private:
    u64 m_Size;
    u8 *m_Buffer;
};
