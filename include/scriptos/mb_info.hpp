#pragma once

#include <scriptos/mmap.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/types.hpp>

class MultibootInfo
{
public:
    class Iter
    {
    public:
        explicit Iter(multiboot_tag *tag);

        multiboot_tag &operator*() const;

        Iter &operator++();
        Iter operator++(int);

        bool operator==(const Iter &other) const;
        bool operator!=(const Iter &other) const;

    private:
        multiboot_tag *m_Tag;
    };

    Iter begin() const;
    Iter end() const;

    multiboot_tag *operator[](u32 type) const;

    MemoryMap GetMMap() const;

private:
    u32 m_TotalSize;
    u32 m_Reserved;
};
