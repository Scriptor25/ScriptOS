#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/mb_info.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/kernel/pit.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/kernel/serial.hpp>
#include <scriptos/kernel/user.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>
#include <scriptos/stl/string.hpp>

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

static void exec(const string &cmd)
{
    string_view v = cmd;
    auto args = v.split(',');
    for (auto &arg : args)
        arg = arg.trim();
    auto command = args.pop_front();

    if (command == "print")
    {
        for (auto &arg : args)
        {
            printn(arg.data(), arg.size());
            putchar(' ');
        }
        putchar('\n');
        return;
    }
    if (command == "panic")
    {
        string message;
        bool first = true;
        for (auto &arg : args)
        {
            if (first)
                first = false;
            else
                message += " ";
            message += arg;
        }
        Panic("%.*s", message.size(), message.data());
    }

    Serial_Write("undefined command ");
    Serial_Write('\'');
    Serial_Write(command.data(), command.size());
    Serial_Write('\'');
    if (!args.empty())
        Serial_Write(", args");
    for (auto &arg : args)
    {
        Serial_Write(" '");
        Serial_Write(arg.data(), arg.size());
        Serial_Write('\'');
    }
    Serial_Write("\r\n");
}

extern "C" void kernel_main(u32 magic, const MultibootInfo &info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((uptr)&info & 7))
        return;

    setup_memory(info);
    setup_graphics(info);

    void *kernel_stack;
    asm volatile("mov %%esp, %0" : "=g"(kernel_stack));

    InitGDT(kernel_stack);
    InitIDT();

    CLI();
    PIC_Remap(PIC1_OFFSET, PIC2_OFFSET);
    PIC_Disable();
    PIC_Clr_Mask(0);
    PIT_Write_C0_w(1193); // 1000Hz
    STI();

    {
        auto &graphics = Graphics::GetInstance();
        {
            u32 data[]{
                0xffff0000,
            };
            graphics.DrawTexture(50, 50, 0.f, 0.f, 150, 150, 1.f, 1.f, 1, 1, data);
        }
        {
            u32 data[]{
                0x9900ff00,
            };
            graphics.DrawTexture(80, 60, 0.f, 0.f, 180, 160, 1.f, 1.f, 1, 1, data);
        }
        {
            u32 data[]{
                0x770000ff,
            };
            graphics.DrawTexture(110, 70, 0.f, 0.f, 210, 170, 1.f, 1.f, 1, 1, data);
        }
    }

    // auto user_stack = PageFrameAllocator::GetInstance().RequestPage();
    // PageTableManager::GetKernelInstance().MapPage((void *)0xC0000000, user_stack, true);

    // auto page_count = ceil_div((uptr)USER_TEXT_END - (uptr)USER_TEXT_START, PAGE_SIZE);
    // PageTableManager::GetKernelInstance().MapPages(USER_TEXT_START, USER_TEXT_START, page_count, true);

    // jump_user_main((void *)(0xC0000000 + PAGE_SIZE));

    auto error = Serial_Init();
    if (error)
    {
        LOOP();
    }

    Serial_Write("Hello Serial Terminal!\r\n");
    Serial_Write("> ");

    string buffer;
    while (true)
    {
        auto c = Serial_Read();
        switch (c)
        {
        case 0x08:
            if (buffer.empty())
                break;
            buffer.pop_back();
            Serial_Write(0x08);
            Serial_Write(' ');
            Serial_Write(0x08);
            break;
        case 0x0d:
            Serial_Write("\r\n");
            exec(buffer);
            buffer.clear();
            Serial_Write("> ");
            break;
        default:
            if (c >= 0x20)
            {
                buffer.push_back(c);
                Serial_Write(c);
            }
            break;
        }
    }
}
