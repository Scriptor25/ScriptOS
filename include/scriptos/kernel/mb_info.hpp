#pragma once

#include <scriptos/kernel/mmap.hpp>
#include <scriptos/kernel/multiboot2.hpp>
#include <scriptos/std/types.hpp>

/**
 * Multiboot info structure wrapper.
 */
class MultibootInfo
{
public:
    template <typename T>
    const T *at(u32 type) const
    {
        const T *result = nullptr;
        ForEach(
            [&](const multiboot_tag *tag) -> bool
            {
                if (tag->type != type)
                    return false;
                result = reinterpret_cast<const T *>(tag);
                return true;
            });
        return result;
    }

    /**
     * Iterate over the multiboot info structure. Return early by returning true from the callback.
     */
    template <typename T>
    void ForEach(T callback) const
    {
        for (auto tag = reinterpret_cast<const multiboot_tag *>(this) + 1;
             tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = reinterpret_cast<const multiboot_tag *>(reinterpret_cast<uptr>(tag) + ((tag->size + 7) & ~7)))
            if (callback(tag))
                return;
    }

    /**
     * Utility function to get a wrapped version of the multiboot memory map info structure.
     */
    MemoryMap GetMMap() const;

private:
    u32 m_TotalSize;
    u32 m_Reserved;
};
