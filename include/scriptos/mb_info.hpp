#pragma once

#include <scriptos/mmap.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/types.hpp>

class MultibootInfo
{
public:
    multiboot_tag *operator[](u32 type) const;

    template <typename T>
    void ForEach(T callback) const
    {
        for (auto tag = (multiboot_tag *)this + 1; tag->type != MULTIBOOT_TAG_TYPE_END; tag = (multiboot_tag *)((uptr)tag + ((tag->size + 7) & ~7)))
            if (callback(tag))
                return;
    }

    MemoryMap GetMMap() const;

private:
    u32 m_TotalSize;
    u32 m_Reserved;
};
