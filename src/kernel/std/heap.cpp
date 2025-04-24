#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/assert.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/util.hpp>

struct heap_header
{
    bool free;
    usize length;
    heap_header* prev;
    heap_header* next;
};

static heap_header* heap_root = nullptr;

static usize heap_size()
{
    usize size = 0;

    for (auto header = heap_root; header; header = header->next)
        size += header->length + sizeof(heap_header);

    return size;
}

static heap_header* heap_end()
{
    heap_header* header;

    for (header = heap_root; header->next; header = header->next)
        ;

    return header;
}

static void init_heap(usize size = 0x10000 /* default 64KiB heap */)
{
    auto& pfa = PageFrameAllocator::GetKernelInstance();
    auto& ptm = PageTableManager::GetKernelInstance();

    heap_header* chunk = nullptr;
    for (usize offset = 0; offset < size; offset += PAGE_SIZE)
    {
        auto address = pfa.RequestPage();
        assert(address && "out of memory");

        ptm.MapPage(address, address);

        auto header = reinterpret_cast<heap_header*>(address);

        if (!heap_root)
            heap_root = header;

        if (chunk)
        {
            if (address == reinterpret_cast<void*>(reinterpret_cast<uptr>(chunk + 1) + chunk->length))
            {
                chunk->length += PAGE_SIZE;
                continue;
            }

            chunk->next = header;
        }

        header->free = true;
        header->length = PAGE_SIZE - sizeof(heap_header);
        header->prev = chunk;
        header->next = nullptr;

        chunk = header;
    }
}

static void expand_heap(usize size)
{
    auto& pfa = PageFrameAllocator::GetKernelInstance();
    auto& ptm = PageTableManager::GetKernelInstance();

    auto aligned_heap_size = ceil_div(heap_size(), PAGE_SIZE) * PAGE_SIZE;
    auto total_size = aligned_heap_size + size;

    auto last_header = heap_end();
    auto chunk = last_header->free ? last_header : nullptr;

    for (usize offset = aligned_heap_size; offset < total_size; offset += PAGE_SIZE)
    {
        auto address = pfa.RequestPage();
        assert(address && "out of memory");

        ptm.MapPage(address, address);

        auto header = reinterpret_cast<heap_header*>(address);

        if (chunk)
        {
            if (address == reinterpret_cast<void*>(reinterpret_cast<uptr>(chunk + 1) + chunk->length))
            {
                chunk->length += PAGE_SIZE;
                continue;
            }

            chunk->next = header;
        }

        header->free = true;
        header->length = PAGE_SIZE - sizeof(heap_header);
        header->prev = chunk ? chunk : last_header;
        header->next = nullptr;

        chunk = header;
    }
}

void* malloc(usize count)
{
    if (!heap_root)
        init_heap();

    heap_header* header;
    for (header = heap_root; header && !(header->free && header->length >= count); header = header->next)
        ;

    if (!header)
    {
        expand_heap(count);
        return malloc(count);
    }

    if ((header->length - count) <= sizeof(heap_header))
    {
        header->free = false;
        return header + 1;
    }

    auto split_header = reinterpret_cast<heap_header*>(reinterpret_cast<uptr>(header + 1) + count);
    split_header->free = true;
    split_header->length = header->length - sizeof(heap_header) - count;
    split_header->prev = header;
    split_header->next = header->next;

    if (split_header->next)
        split_header->next->prev = split_header;

    header->free = false;
    header->length = count;
    header->next = split_header;

    return header + 1;
}

static bool adjacent(heap_header* header_a, heap_header* header_b)
{
    return reinterpret_cast<void*>(reinterpret_cast<uptr>(header_a + 1) + header_a->length) == reinterpret_cast<void*>(header_b);
}

void free(void* address)
{
    if (!address)
        return;

    auto header = reinterpret_cast<heap_header*>(address) - 1;
    header->free = true;

    if (header->prev && header->prev->free && adjacent(header->prev, header))
    {
        header->prev->length = header->prev->length + sizeof(heap_header) + header->length;
        header->prev->next = header->next;

        if (header->next)
            header->next->prev = header->prev;

        header = header->prev;
    }

    if (header->next && header->next->free && adjacent(header, header->next))
    {
        header->length = header->length + sizeof(heap_header) + header->next->length;
        header->next = header->next->next;

        if (header->next)
            header->next->prev = header;
    }
}

void* realloc(void* address, usize count)
{
    if (!count)
    {
        free(address);
        return nullptr;
    }

    auto new_address = malloc(count);

    if (!address)
        return new_address;

    auto header = reinterpret_cast<heap_header*>(address) - 1;

    if (new_address)
        memcpy(new_address, address, header->length);

    free(address);

    return new_address;
}

void* calloc(usize count, usize size)
{
    auto address = malloc(count * size);

    if (address)
        memset(address, 0, count * size);

    return address;
}
