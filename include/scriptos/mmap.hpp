#pragma once

#include <scriptos/multiboot2.hpp>
#include <scriptos/types.hpp>

class MemoryMap
{
public:
    class Iter
    {
    public:
        Iter(const multiboot_mmap_entry *ptr, u32 entry_size);

        const multiboot_mmap_entry &operator*() const;

        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        const multiboot_mmap_entry *m_Ptr;
        u32 m_EntrySize;
    };

    MemoryMap();
    MemoryMap(const multiboot_mmap_entry *beg, const multiboot_mmap_entry *end, u32 entry_size);

    u64 Size();
    u64 Size() const;

    Iter begin() const;
    Iter end() const;

private:
    const multiboot_mmap_entry *m_Beg;
    const multiboot_mmap_entry *m_End;

    u32 m_EntrySize;
    u64 m_Size = 0;
};
