#include <scriptos/memory.h>
#include <scriptos/paging.h>

struct HeapHeader
{
    bool free;
    usize size;
    HeapHeader* left;
    HeapHeader* right;
} __attribute__((packed));

static paging::PageFrameAllocator* __heap_allocator = nullptr;
static HeapHeader* __heap_begin = nullptr;
static usize __heap_size = 0;

static HeapHeader* __heap_end()
{
    auto header = __heap_begin;
    while (header->right)
        header = header->right;
    return header;
}

static HeapHeader* __merge(
    HeapHeader* left,
    HeapHeader* right)
{
    if (left->right != right || right->left != left)
        return left;

    if (!left->free || !right->free)
        return left;

    if (reinterpret_cast<HeapHeader*>(reinterpret_cast<uptr>(left + 1) + left->size) != right)
        return left;

    left->size += sizeof(HeapHeader) + right->size;
    left->right = right->right;

    if (left->right)
        left->right->left = left;

    return left;
}

static void __extend_heap()
{
    auto page_count = __heap_size / PAGE_SIZE + 1;

    auto physical_address = __heap_allocator->AllocatePhysicalPages(page_count);
    auto virtual_address = paging::PhysicalToVirtual<HeapHeader*>(physical_address);

    paging::MapPages(*__heap_allocator, virtual_address, physical_address, page_count, true, true);

    auto heap_extension = virtual_address;

    auto end = __heap_end();
    end->right = heap_extension;

    heap_extension->free = true;
    heap_extension->size = __heap_size;
    heap_extension->left = end;
    heap_extension->right = nullptr;

    __heap_size <<= 1;

    __merge(end, heap_extension);
}

void memory::InitializeHeap(
    paging::PageFrameAllocator& allocator,
    usize size)
{
    __heap_allocator = &allocator;

    auto page_count = size / PAGE_SIZE + 1;

    auto physical_address = __heap_allocator->AllocatePhysicalPages(page_count);
    auto virtual_address = paging::PhysicalToVirtual<HeapHeader*>(physical_address);

    paging::MapPages(*__heap_allocator, virtual_address, physical_address, page_count, true, true);

    __heap_begin = virtual_address;
    __heap_size = size;

    __heap_begin->free = true;
    __heap_begin->size = size - sizeof(HeapHeader);
    __heap_begin->left = nullptr;
    __heap_begin->right = nullptr;
}

void* memory::Allocate(usize count)
{
    if (!count)
        return nullptr;

    HeapHeader* header = nullptr;
    do
    {
        header = __heap_begin;
        while (header && (!header->free || header->size < count))
            header = header->right;

        if (!header)
            __extend_heap();

    } while (!header);

    if (header->size - sizeof(HeapHeader) <= count)
    {
        header->free = false;
        return header + 1;
    }

    auto right_header = reinterpret_cast<HeapHeader*>(reinterpret_cast<uptr>(header + 1) + count);

    right_header->free = true;
    right_header->size = header->size - sizeof(HeapHeader);
    right_header->left = header;
    right_header->right = header->right;

    if (right_header->right)
        right_header->right->left = right_header;

    header->free = false;
    header->size = count;
    header->right = right_header;

    return header + 1;
}

void* memory::Reallocate(
    void* block,
    usize count)
{
    if (!count)
    {
        Free(block);
        return nullptr;
    }

    auto new_block = Allocate(count);
    if (!new_block)
        return nullptr;

    if (block)
    {
        auto header = reinterpret_cast<HeapHeader*>(block) - 1;
        Copy(new_block, block, count < header->size ? count : header->size);
        Free(block);
    }

    return new_block;
}

void memory::Free(void* block)
{
    if (!block)
        return;

    auto header = reinterpret_cast<HeapHeader*>(block) - 1;
    header->free = true;

    if (header->left && header->left->free)
        header = __merge(header->left, header);

    if (header->right && header->right->free)
        header = __merge(header, header->right);
}
