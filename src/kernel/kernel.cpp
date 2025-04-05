#include <scriptos/kernel/acpi.hpp>
#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/mb_info.hpp>
#include <scriptos/kernel/nmi.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/kernel/pci.hpp>
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

    auto tag = info.at<multiboot_tag_framebuffer>(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);
    auto fb_addr = tag->framebuffer_addr_lo;
    auto width = tag->framebuffer_width;
    auto height = tag->framebuffer_height;
    auto pitch = tag->framebuffer_pitch;
    auto bpp = tag->framebuffer_bpp;

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

static void __attribute__((aligned(PAGE_SIZE))) user_main()
{
    LOOP();
}

static void setup_user()
{
    auto &ptm = PageTableManager::GetKernelInstance();

    auto stack = PageFrameAllocator::GetInstance().RequestPage();
    auto main = reinterpret_cast<void *>(::user_main);

    auto mapped_stack = reinterpret_cast<void *>(0xE0000000);
    auto mapped_main = reinterpret_cast<void *>(0xF0000000);

    ptm.MapPage(mapped_stack, stack, true);
    ptm.MapPage(mapped_main, main, true);

    // TODO: load user code into memory and map the used memory regions to be accessible by user mode

    jump_user(reinterpret_cast<void *>(reinterpret_cast<uptr>(mapped_stack) + PAGE_SIZE), mapped_main);
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

    {
        const ACPI::RSDP *rsdp = nullptr;

        auto tag_new = info.at<multiboot_tag_new_acpi>(MULTIBOOT_TAG_TYPE_ACPI_NEW);
        auto tag_old = info.at<multiboot_tag_old_acpi>(MULTIBOOT_TAG_TYPE_ACPI_OLD);

        if (tag_new)
            rsdp = reinterpret_cast<const ACPI::RSDP *>(tag_new->rsdp);
        else if (tag_old)
            rsdp = reinterpret_cast<const ACPI::RSDP *>(tag_old->rsdp);
        else
            print("no rsdp\n");

        if (rsdp)
        {
            auto rsdt = reinterpret_cast<ACPI::RSDT *>(rsdp->RSDTAddress);
            auto mcfg = reinterpret_cast<ACPI::MCFG_Header *>(rsdt->Find("MCFG"));
            PCI::EnumeratePCI(mcfg);
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
