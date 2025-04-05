#include <scriptos/kernel/mb_info.hpp>

MemoryMap MultibootInfo::GetMMap() const
{
    auto tag = this->at<multiboot_tag_mmap>(MULTIBOOT_TAG_TYPE_MMAP);
    return MemoryMap(tag->entries, reinterpret_cast<const multiboot_mmap_entry *>(reinterpret_cast<uptr>(tag) + tag->size), tag->entry_size);
}
