#include <scriptos/bitmap.hpp>
#include <scriptos/font.hpp>
#include <scriptos/framebuffer.hpp>
#include <scriptos/gdt.hpp>
#include <scriptos/graphics.hpp>
#include <scriptos/idt.hpp>
#include <scriptos/info.hpp>
#include <scriptos/interrupts.hpp>
#include <scriptos/io.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/paging.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>
#include <scriptos/ptm.hpp>
#include <scriptos/types.hpp>
#include <scriptos/util.hpp>

#define KiB(BYTES) (BYTES / 1024)
#define MiB(BYTES) (BYTES / (1024 * 1024))
#define GiB(BYTES) (BYTES / (1024 * 1024 * 1024))
#define TiB(BYTES) (BYTES / (1024 * 1024 * 1024 * 1024))

static PageTableManager ptm;

static void setup_memory(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::Get();

    auto &tag = *(multiboot_tag_mmap *)info[MULTIBOOT_TAG_TYPE_MMAP];
    MemoryMap mmap(tag.entries, (const multiboot_mmap_entry *)((u8 *)&tag + tag.size), tag.entry_size);

    pfa.Init(mmap);
    pfa.LockPages(KERNEL_START, ceil_div<usize>((uptr)KERNEL_END - (uptr)KERNEL_START, PAGE_SIZE));

    ptm = PageTableManager((PageDirectoryEntry *)pfa.RequestEmptyPage());
    ptm.MapPages(nullptr, nullptr, ceil_div<usize>(mmap.Size(), PAGE_SIZE));
    ptm.SetupPaging();
}

static void setup_graphics(const MultibootInfo &info)
{
    auto &graphics = Graphics::Get();
    auto &pfa = PageFrameAllocator::Get();

    auto &tag = *(multiboot_tag_framebuffer *)info[MULTIBOOT_TAG_TYPE_FRAMEBUFFER];
    auto fb_addr = tag.framebuffer_addr;
    auto width = tag.framebuffer_width;
    auto height = tag.framebuffer_height;
    auto pitch = tag.framebuffer_pitch;
    auto bpp = tag.framebuffer_bpp;

    auto page_count = ceil_div<usize>(pitch * height, PAGE_SIZE);
    pfa.LockPages((void *)fb_addr, page_count);
    ptm.MapPages((void *)fb_addr, (void *)fb_addr, page_count);

    auto bb_addr = pfa.RequestPages(page_count);

    graphics.Init((u8 *)fb_addr, (u8 *)bb_addr, width, height, pitch, bpp);

    graphics.Clear(0xff121212);
    graphics.Reset();
    graphics.SetCharColor(0xfffefefe);
}

extern "C" void kernel_main(u32 magic, const MultibootInfo &info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((uptr)&info & 7))
        return;

    setup_memory(info);
    setup_graphics(info);

    InitGDT();
    InitIDT();
    RemapPIC();

    outb(PIC1_DATA, 0b11111101);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    asm volatile("sti");

    print("Hello World!\n");

    for (;;)
        Graphics::Get().SwapBuffers();
}
