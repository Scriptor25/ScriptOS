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
#include <scriptos/stl/string_view.hpp>

static void setup_memory(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    auto mmap = info.GetMMap();

    pfa.Initialize(mmap);
    pfa.LockPages(KERNEL_START, ceil_div(KERNEL_SIZE, PAGE_SIZE));

    ptm.Initialize(reinterpret_cast<PageDirectoryEntry *>(pfa.RequestEmptyPage()));
    ptm.MapPages(nullptr, nullptr, ceil_div(mmap.Size(), PAGE_SIZE));
    ptm.SetupPaging();
}

static void setup_graphics(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();
    auto &graphics = Graphics::GetInstance();

    auto &tag = *reinterpret_cast<const multiboot_tag_framebuffer *>(info[MULTIBOOT_TAG_TYPE_FRAMEBUFFER]);
    auto fb_addr = tag.framebuffer_addr_lo;
    auto width = tag.framebuffer_width;
    auto height = tag.framebuffer_height;
    auto pitch = tag.framebuffer_pitch;
    auto bpp = tag.framebuffer_bpp;

    auto page_count = ceil_div(pitch * height, PAGE_SIZE);
    pfa.LockPages(reinterpret_cast<void *>(fb_addr), page_count);
    ptm.MapPages(reinterpret_cast<void *>(fb_addr), reinterpret_cast<void *>(fb_addr), page_count);

    auto bb_addr = malloc(pitch * height);
    graphics.Initialize(reinterpret_cast<u8 *>(fb_addr), reinterpret_cast<u8 *>(bb_addr), width, height, pitch, bpp);

    graphics.SetBGColor(0xff121212);
    graphics.SetFGColor(0xfffefefe);
    graphics.Reset();
    graphics.Clear();
}

/**
 * - request a free page as user stack space and map it to some "anonymous" virtual address
 * - map the text, rodata, data and bss sections to be accessible by the user mode
 * - jump to the user mode entry point
 */
static void setup_user()
{
    auto user_stack = PageFrameAllocator::GetInstance().RequestPage();
    PageTableManager::GetKernelInstance().MapPage(reinterpret_cast<void *>(0xC0000000), user_stack, true);

    auto text_page_count = ceil_div(USER_TEXT_SIZE, PAGE_SIZE);
    PageTableManager::GetKernelInstance().MapPages(USER_TEXT_START, USER_TEXT_START, text_page_count, true);

    auto rodata_page_count = ceil_div(USER_RODATA_SIZE, PAGE_SIZE);
    PageTableManager::GetKernelInstance().MapPages(USER_RODATA_START, USER_RODATA_START, rodata_page_count, true);

    auto data_page_count = ceil_div(USER_DATA_SIZE, PAGE_SIZE);
    PageTableManager::GetKernelInstance().MapPages(USER_DATA_START, USER_DATA_START, data_page_count, true);

    auto bss_page_count = ceil_div(USER_BSS_SIZE, PAGE_SIZE);
    PageTableManager::GetKernelInstance().MapPages(USER_BSS_START, USER_BSS_START, bss_page_count, true);

    jump_user_main(reinterpret_cast<void *>(0xC0000000 + PAGE_SIZE));
}

static void serial_exec(const string &cmd)
{
    auto args = string_view(cmd).split(',');
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
                message += ' ';
            message += arg;
        }
        Panic("%.*s", message.size(), message.data());
    }

    Serial::Write("undefined command ");
    Serial::Write('\'');
    Serial::Write(command.data(), command.size());
    Serial::Write('\'');
    if (!args.empty())
        Serial::Write(", args");
    for (auto &arg : args)
    {
        Serial::Write(" '");
        Serial::Write(arg.data(), arg.size());
        Serial::Write('\'');
    }
    Serial::Write("\r\n");
}

extern "C" void kernel_main(u32 magic, const MultibootInfo &info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || (reinterpret_cast<uptr>(&info) & 7))
        return;

    setup_memory(info);
    setup_graphics(info);

    /**
     * Get the current kernel stack address for initializing the GDT
     */
    void *kernel_stack;
    asm volatile("mov %%esp, %0" : "=g"(kernel_stack));

    GDT::Initialize(kernel_stack);
    IDT::Initialize();

    CLI();
    PIC::Remap(PIC1_OFFSET, PIC2_OFFSET);
    PIC::Disable();
    PIC::Clr_Mask(0);
    PIT::Write_C0_w(1193); // 1000Hz
    STI();

    /**
     * Draw some test graphics
     */
    {
        auto &graphics = Graphics::GetInstance();
        {
            u32 data[]{
                0xffff0000,
            };
            graphics.DrawTexture(50, 50, 0.f, 0.f, 150, 150, 1.f, 1.f, 1, 1, false, data);
        }
        {
            u32 data[]{
                0x9900ff00,
            };
            graphics.DrawTexture(80, 60, 0.f, 0.f, 180, 160, 1.f, 1.f, 1, 1, false, data);
        }
        {
            u32 data[]{
                0x770000ff,
            };
            graphics.DrawTexture(110, 70, 0.f, 0.f, 210, 170, 1.f, 1.f, 1, 1, false, data);
        }
        {
            u32 data[]{
                0xffff0000,
                0xff00ff00,
                0xff0000ff,
                0xffffffff,
            };
            graphics.DrawTexture(400, 50, 0.f, 0.f, 500, 150, 1.f, 1.f, 2, 2, true, data);
            graphics.DrawString(400, 150, 100, L"this is an image with some text äöü");
        }
    }

    setup_user();

    auto error = Serial::Initialize();
    if (error)
        LOOP();

    Serial::Write("Hello Serial Terminal!\r\n");
    Serial::Write("> ");

    string buffer;
    while (true)
    {
        auto c = Serial::Read();
        switch (c)
        {
        case 0x08:
            if (buffer.empty())
                break;
            buffer.pop_back();
            Serial::Write(0x08);
            Serial::Write(' ');
            Serial::Write(0x08);
            break;
        case 0x0d:
            Serial::Write("\r\n");
            serial_exec(buffer);
            buffer.clear();
            Serial::Write("> ");
            break;
        default:
            if (c >= 0x20)
            {
                buffer.push_back(c);
                Serial::Write(c);
            }
            break;
        }
    }
}
