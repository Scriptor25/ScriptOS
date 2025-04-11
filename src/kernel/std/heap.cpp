#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/assert.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/util.hpp>

struct heap_header
{
    bool free;
    usize length;
    heap_header *prev;
    heap_header *next;
};

static heap_header *heap_root = nullptr;

static usize heap_size()
{
    usize size = 0;

    for (auto header = heap_root; header; header = header->next)
        size += header->length + sizeof(heap_header);

    return size;
}

static heap_header *heap_end()
{
    heap_header *header;

    for (header = heap_root; header->next; header = header->next)
        ;

    return header;
}

static void init_heap(usize size = 0x10000 /* default 64KiB heap */)
{
    auto &pfa = PageFrameAllocator::GetKernelInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    for (usize offset = 0; offset < size; offset += PAGE_SIZE)
    {
        auto physical_address = pfa.RequestPage();
        assert(physical_address && "out of memory");

        if (!heap_root)
            heap_root = reinterpret_cast<heap_header *>(physical_address);

        auto virtual_address = reinterpret_cast<void *>(reinterpret_cast<uptr>(heap_root) + offset);
        ptm.MapPage(virtual_address, physical_address);
    }

    heap_root->free = true;
    heap_root->length = size - sizeof(heap_header);
    heap_root->prev = nullptr;
    heap_root->next = nullptr;
}

static heap_header *expand_heap(usize size)
{
    auto &pfa = PageFrameAllocator::GetKernelInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    auto aligned_heap_size = ceil_div(heap_size(), PAGE_SIZE) * PAGE_SIZE;
    auto total_size = aligned_heap_size + size;

    for (usize offset = aligned_heap_size; offset < total_size; offset += PAGE_SIZE)
    {
        auto address = pfa.RequestPage();
        assert(address && "out of memory");

        ptm.MapPage(reinterpret_cast<void *>(reinterpret_cast<uptr>(heap_root) + offset), address);
    }

    auto header = heap_end();

    if (header->free)
    {
        header->length = header->length + size;
    }
    else
    {
        auto new_header = reinterpret_cast<heap_header *>(reinterpret_cast<uptr>(header + 1) + header->length);

        new_header->free = true;
        new_header->length = size - sizeof(heap_header);
        new_header->prev = header;
        new_header->next = nullptr;

        header->next = new_header;
        header = new_header;
    }

    return header;
}

void *malloc(usize count)
{
    if (!heap_root)
        init_heap();

    heap_header *header;
    for (header = heap_root; header && !(header->free && header->length >= count); header = header->next)
        ;

    if (!header)
        header = expand_heap(count);

    if (!header)
        return nullptr;

    if (header->length - count <= sizeof(heap_header))
    {
        header->free = false;
        return header + 1;
    }

    auto split = reinterpret_cast<heap_header *>(reinterpret_cast<uptr>(header + 1) + count);
    split->free = true;
    split->length = header->length - sizeof(heap_header) - count;
    split->prev = header;
    split->next = header->next;

    if (split->next)
        split->next->prev = split;

    header->free = false;
    header->length = count;
    header->next = split;

    return header + 1;
}

void free(void *address)
{
    if (!address)
        return;

    auto header = reinterpret_cast<heap_header *>(address) - 1;
    header->free = true;

    if (header->prev && header->prev->free)
    {
        header->prev->length = header->prev->length + sizeof(heap_header) + header->length;
        header->prev->next = header->next;

        if (header->next)
            header->next->prev = header->prev;

        header = header->prev;
    }

    if (header->next && header->next->free)
    {
        header->length = header->length + sizeof(heap_header) + header->next->length;
        header->next = header->next->next;

        if (header->next)
            header->next->prev = header;
    }
}

void *realloc(void *address, usize count)
{
    if (!count)
    {
        free(address);
        return nullptr;
    }

    auto new_address = malloc(count);

    if (!address)
        return new_address;

    auto header = reinterpret_cast<heap_header *>(address) - 1;

    if (new_address)
        memcpy(new_address, address, header->length);

    free(address);

    return new_address;
}

void *calloc(usize count, usize size)
{
    auto address = malloc(count * size);

    if (address)
        memset(address, 0, count * size);

    return address;
}
