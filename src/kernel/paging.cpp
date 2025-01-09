#include <scriptos/paging.hpp>
#include <scriptos/print.hpp>

static PageEntry pd[0x400] alignas(0x1000);
static PageEntry pt[0x400] alignas(0x1000);

void reset_paging()
{
    pd[0].PDE = {
        .Present = 1,
        .Read_Write = 1,
        .Address_31_12 = (uptr)pt >> 12,
    };
    for (u32 i = 1; i < 0x400; ++i)
        pd[i].Data = 0x0;
    for (u32 i = 0; i < 0x400; ++i)
        pt[i].PTE = {
            .Present = 1,
            .Read_Write = 1,
            .Address_31_12 = (i * 0x1000) >> 12,
        };
}

void enable_paging()
{
    asm volatile(
        "mov %0, %%eax;"
        "mov %%eax, %%cr3;"
        "mov %%cr0, %%eax;"
        "or $0x80000001, %%eax;"
        "mov %%eax, %%cr0;"
        :
        : "r"(pd));
}

void print_cr3()
{
    u32 val;
    asm volatile(
        "mov %%cr3, %%eax;"
        "mov %%eax, %0;"
        : "=r"(val)
        :);
    printf("cr3 = %p\n", (void *)(uptr)val);
}
