#pragma once

#include <scriptos/kernel/multiboot2.hpp>
#include <scriptos/std/types.hpp>

/**
 * Multiboot memory map structure wrapper
 */
class MemoryMap
{
public:
    class Iter
    {
    public:
        Iter(const multiboot_mmap_entry *ptr, usize entry_size);

        const multiboot_mmap_entry &operator*() const;

        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        const multiboot_mmap_entry *m_Ptr;
        usize m_EntrySize;
    };

    /**
     * Create an empty memory map.
     */
    MemoryMap();
    /**
     * Create a memory map wrapping around a multiboot memory map with the given entry size.
     */
    MemoryMap(const multiboot_mmap_entry *beg, const multiboot_mmap_entry *end, usize entry_size);

    /**
     * Get the total memory size in bytes.
     */
    usize Size() const;

    /**
     * Get an iterator from the first memory map entry.
     */
    Iter begin() const;
    /**
     * Get an iterator after the last memory map entry.
     */
    Iter end() const;

private:
    const multiboot_mmap_entry *m_Beg;
    const multiboot_mmap_entry *m_End;

    usize m_EntrySize;
    usize m_Size;
};
