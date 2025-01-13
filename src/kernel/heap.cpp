#include <scriptos/std/memory.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/ptm.hpp>
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
    for (auto ptr = heap_root; ptr; ptr = ptr->next)
        size += ptr->length + sizeof(heap_header);
    return size;
}

static heap_header *heap_end()
{
    for (auto ptr = heap_root; ptr; ptr = ptr->next)
        if (!ptr->next)
            return ptr;
    return nullptr;
}

static void init_heap(usize size = 0x100000 /* default 1MiB heap */)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    heap_root = (heap_header *)pfa.RequestPage();
    for (usize i = PAGE_SIZE; i < size; i += PAGE_SIZE)
    {
        auto address = pfa.RequestPage();
        ptm.MapPage((void *)((uptr)heap_root + i), address);
    }

    heap_root->free = true;
    heap_root->length = size - sizeof(heap_header);
    heap_root->prev = nullptr;
    heap_root->next = nullptr;
}

static heap_header *expand_heap(usize size)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    auto aligned_heap_size = ceil_div(heap_size(), PAGE_SIZE) * PAGE_SIZE;
    auto total_size = aligned_heap_size + size;
    for (usize i = aligned_heap_size; i < total_size; i += PAGE_SIZE)
    {
        auto address = pfa.RequestPage();
        ptm.MapPage((void *)((uptr)heap_root + i), address);
    }

    auto ptr = heap_end();
    if (ptr->free)
        ptr->length = ptr->length + size;
    else
    {
        ptr->next = (heap_header *)((uptr)(ptr + 1) + ptr->length);

        ptr->next->free = true;
        ptr->next->length = size - sizeof(heap_header);
        ptr->next->prev = ptr;
        ptr->next->next = nullptr;

        ptr = ptr->next;
    }

    return ptr;
}

void *malloc(usize count)
{
    if (!heap_root)
        init_heap();

    heap_header *ptr;
    for (ptr = heap_root; ptr; ptr = ptr->next)
        if (ptr->free && ptr->length >= count)
            break;
    if (!ptr)
        ptr = expand_heap(count * 2);

    if (ptr->length - count <= sizeof(heap_header))
    {
        ptr->free = false;
        return ptr + 1;
    }

    auto split = (heap_header *)((uptr)(ptr + 1) + count);
    split->free = true;
    split->length = ptr->length - sizeof(heap_header) - count;
    split->prev = ptr;
    split->next = ptr->next;
    if (split->next)
        split->next->prev = split;

    ptr->free = false;
    ptr->length = count;
    ptr->next = split;

    return ptr + 1;
}

void free(void *address)
{
    auto ptr = (heap_header *)address - 1;
    ptr->free = true;

    if (ptr->prev && ptr->prev->free)
    {
        ptr->prev->length = ptr->prev->length + sizeof(heap_header) + ptr->length;
        ptr->prev->next = ptr->next;
        if (ptr->next)
            ptr->next->prev = ptr->prev;
        ptr = ptr->prev;
    }

    if (ptr->next && ptr->next->free)
    {
        ptr->length = ptr->length + sizeof(heap_header) + ptr->next->length;
        ptr->next = ptr->next->next;
        if (ptr->next)
            ptr->next->prev = ptr;
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

    auto ptr = (heap_header *)address - 1;
    memcpy(new_address, address, ptr->length);
    free(address);

    return new_address;
}

void *calloc(usize count, usize size)
{
    auto address = malloc(count * size);
    memset(address, 0, count * size);
    return address;
}
