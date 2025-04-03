#pragma once

#include <scriptos/std/types.hpp>

/**
 * Immutable structure representation of a bit inside the bitmap
 */
struct BitEntry
{
    u32 ByteIndex;
    u32 BitIndex;

    bool Set;
};

/**
 * A bitmap, storing bits as a row-column addressable map
 */
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

    /**
     * Initialize the bitmap with a size (in byte) and a pre-allocated buffer.
     */
    void Initialize(usize size, u8 *buffer);

    /**
     * Get the size (in byte).
     */
    usize Size() const;
    /**
     * Get the underlying buffer.
     */
    void *Buffer() const;

    /**
     * Get a bit.
     */
    bool operator[](usize index) const;
    /**
     * Get a bit.
     */
    bool Get(usize index) const;
    /**
     * Set a bit.
     */
    bool Set(usize index);
    /**
     * Clear a bit.
     */
    bool Clr(usize index);

    /**
     * Get an iterator from the first bit.
     */
    Iter begin() const;
    /**
     * Get an iterator after the last bit.
     */
    Iter end() const;

private:
    usize m_Size;
    u8 *m_Buffer;
};
