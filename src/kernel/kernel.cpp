#include <scriptos/kernel/acpi.hpp>
#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/mb_info.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

static void setup_memory(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    auto mmap = info.GetMMap();

    pfa.Init(mmap);
    pfa.LockPages(KERNEL_START, ceil_div((uptr)KERNEL_END - (uptr)KERNEL_START, PAGE_SIZE));

    ptm.Init((PageDirectoryEntry *)pfa.RequestEmptyPage());
    ptm.MapPages(nullptr, nullptr, ceil_div(mmap.Size(), PAGE_SIZE));
    ptm.SetupPaging();
}

static void setup_graphics(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();
    auto &graphics = Graphics::GetInstance();

    auto &tag = *(multiboot_tag_framebuffer *)info[MULTIBOOT_TAG_TYPE_FRAMEBUFFER];
    auto fb_addr = tag.framebuffer_addr_lo;
    auto width = tag.framebuffer_width;
    auto height = tag.framebuffer_height;
    auto pitch = tag.framebuffer_pitch;
    auto bpp = tag.framebuffer_bpp;

    auto page_count = ceil_div(pitch * height, PAGE_SIZE);
    pfa.LockPages((void *)fb_addr, page_count);
    ptm.MapPages((void *)fb_addr, (void *)fb_addr, page_count);

    auto bb_addr = malloc(pitch * height);
    graphics.Init((u8 *)fb_addr, (u8 *)bb_addr, width, height, pitch, bpp);

    graphics.SetBGColor(0xff121212);
    graphics.SetFGColor(0xfffefefe);
    graphics.Reset();
    graphics.Clear();
}

static void draw_memory_diagram()
{
    auto &graphics = Graphics::GetInstance();
    auto &[px, py] = graphics.Pos();

    auto &pfa = PageFrameAllocator::GetInstance();
    auto &page_map = pfa.PageMap();

    auto w = graphics.Width();

    for (auto [byte_, bit_, set_] : page_map)
    {
        auto x = px + byte_;
        auto y = py + bit_;
        auto color = set_ ? 0xffff0000 : 0xff00ff00;

        y += CHAR_H * (x / w);
        x %= w;

        graphics.DrawPixel(x, y, color);
    }

    auto s = page_map.Size();
    py += CHAR_H * ceil_div(s, w);
}

extern "C" void kernel_main(u32 magic, const MultibootInfo &info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((uptr)&info & 7))
        return;

    setup_memory(info);
    setup_graphics(info);

    InitGDT();
    InitIDT();
    PIC_Remap();
    PIC_Clr_All();

    auto mmap = info.GetMMap();
    for (auto &entry : mmap)
    {
        cstr type = nullptr;
        switch (entry.type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            type = "available";
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            type = "reserved";
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            type = "acpi reclaimable";
            break;
        case MULTIBOOT_MEMORY_NVS:
            type = "nvs";
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            type = "badram";
            break;
        }

        printf("base = %p%p, length = %#08x%08x, type = '%s'\n", entry.base_addr_hi, entry.base_addr_lo, entry.length_hi, entry.length_lo, type);
    }

    auto tag_old_acpi = (multiboot_tag_old_acpi *)info[MULTIBOOT_TAG_TYPE_ACPI_OLD];
    if (tag_old_acpi)
    {
        auto rsdp = (RSDP *)tag_old_acpi->rsdp;
        auto valid = rsdp->Validate();

        printf("rsdp at %p - %s\n", rsdp, valid ? "valid" : "invalid");

        if (valid)
        {
            printf("signature = %.8s\n", rsdp->Signature);
            printf("oemid = %.6s\n", rsdp->OEMID);
        }
    }

    auto tag_new_acpi = (multiboot_tag_new_acpi *)info[MULTIBOOT_TAG_TYPE_ACPI_NEW];
    if (tag_new_acpi)
    {
        auto xsdp = (XSDP *)tag_new_acpi->rsdp;
        printf("xsdp at %p - %s\n", xsdp, xsdp->Validate() ? "valid" : "invalid");
    }

    for (;;)
        Graphics::GetInstance().SwapBuffers();
}
