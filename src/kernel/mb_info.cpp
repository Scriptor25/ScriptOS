#include <scriptos/kernel/mb_info.hpp>

multiboot_tag *MultibootInfo::operator[](u32 type) const
{
    multiboot_tag *result = nullptr;
    ForEach(
        [&](multiboot_tag *tag) -> bool
        {
            if (tag->type != type)
                return false;
            result = tag;
            return true;
        });
    return result;
}

MemoryMap MultibootInfo::GetMMap() const
{
    auto &tag = *(multiboot_tag_mmap *)((*this)[MULTIBOOT_TAG_TYPE_MMAP]);
    return MemoryMap(tag.entries, (multiboot_mmap_entry *)((uptr)&tag + tag.size), tag.entry_size);
}
