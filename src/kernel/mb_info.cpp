#include <scriptos/kernel/mb_info.hpp>

const multiboot_tag *MultibootInfo::operator[](u32 type) const
{
    const multiboot_tag *result = nullptr;
    ForEach(
        [&](const multiboot_tag *tag) -> bool
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
    auto &tag = *reinterpret_cast<const multiboot_tag_mmap *>((*this)[MULTIBOOT_TAG_TYPE_MMAP]);
    return MemoryMap(tag.entries, reinterpret_cast<const multiboot_mmap_entry *>(reinterpret_cast<uptr>(&tag) + tag.size), tag.entry_size);
}
