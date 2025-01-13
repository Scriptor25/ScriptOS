#pragma once

#include <scriptos/kernel/multiboot2.hpp>
#include <scriptos/std/types.hpp>

class MemoryMap
{
public:
    class Iter
    {
    public:
        Iter(multiboot_mmap_entry *ptr, usize entry_size);

        multiboot_mmap_entry &operator*() const;

        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        multiboot_mmap_entry *m_Ptr;
        usize m_EntrySize;
    };

    MemoryMap();
    MemoryMap(multiboot_mmap_entry *beg, multiboot_mmap_entry *end, usize entry_size);

    usize Size();
    usize Size() const;

    Iter begin() const;
    Iter end() const;

private:
    multiboot_mmap_entry *m_Beg;
    multiboot_mmap_entry *m_End;

    usize m_EntrySize;
    usize m_Size;
};
