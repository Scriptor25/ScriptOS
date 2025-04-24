#include <scriptos/kernel/acpi.hpp>
#include <scriptos/kernel/ata.hpp>
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
#include <scriptos/std/stream.hpp>
#include <scriptos/std/time.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>
#include <scriptos/stl/string.hpp>
#include <scriptos/stl/string_view.hpp>

static void setup_memory(const MultibootInfo& info)
{
    auto& pfa = PageFrameAllocator::GetKernelInstance();
    auto& ptm = PageTableManager::GetKernelInstance();

    auto mmap = info.GetMMap();

    pfa.Initialize(mmap);
    pfa.LockPages(KERNEL_START, ceil_div(KERNEL_SIZE, PAGE_SIZE));

    auto page_directory = reinterpret_cast<PageDirectoryEntry*>(pfa.RequestEmptyPage());
    assert(page_directory && "out of memory");

    ptm.Initialize(page_directory);
    ptm.MapPages(nullptr, nullptr, ceil_div(mmap.Size(), PAGE_SIZE));
    ptm.SetupPaging();
}

static void setup_graphics(const MultibootInfo& info)
{
    auto& pfa = PageFrameAllocator::GetKernelInstance();
    auto& ptm = PageTableManager::GetKernelInstance();
    auto& graphics = Graphics::GetKernelInstance();

    auto tag = info.at<multiboot_tag_framebuffer>(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);
    auto fb_addr = tag->framebuffer_addr_lo;
    auto width = tag->framebuffer_width;
    auto height = tag->framebuffer_height;
    auto pitch = tag->framebuffer_pitch;
    auto bpp = tag->framebuffer_bpp;

    auto page_count = ceil_div(pitch * height, PAGE_SIZE);
    pfa.LockPages(reinterpret_cast<void*>(fb_addr), page_count);
    ptm.MapPages(reinterpret_cast<void*>(fb_addr), reinterpret_cast<void*>(fb_addr), page_count);

    auto bb_addr = pfa.RequestPages(page_count);
    ptm.MapPages(bb_addr, bb_addr, page_count);
    graphics.Initialize(reinterpret_cast<u8*>(fb_addr), reinterpret_cast<u8*>(bb_addr), width, height, pitch, bpp);

    graphics.SetBGColor(0xff121212);
    graphics.SetFGColor(0xfffefefe);
    graphics.Reset();
    graphics.Clear();
}

static void setup_pci(const MultibootInfo& info)
{
    const ACPI::RSDP* rsdp = nullptr;

    if (auto tag = info.at<multiboot_tag_new_acpi>(MULTIBOOT_TAG_TYPE_ACPI_NEW); tag && !rsdp)
        rsdp = reinterpret_cast<const ACPI::RSDP*>(tag->rsdp);
    if (auto tag = info.at<multiboot_tag_old_acpi>(MULTIBOOT_TAG_TYPE_ACPI_OLD); tag && !rsdp)
        rsdp = reinterpret_cast<const ACPI::RSDP*>(tag->rsdp);

    if (!rsdp)
    {
        puts("no rsdp\n");
        return;
    }

    if (!rsdp->Validate())
    {
        puts("failed to validate rsdp\n");
        return;
    }

    auto rsdt = reinterpret_cast<ACPI::RSDT*>(rsdp->RSDTAddress);
    auto mcfg = reinterpret_cast<ACPI::MCFG_Header*>(rsdt->Find("MCFG"));

    if (!mcfg)
    {
        puts("no mcfg in rsdp\n");
        return;
    }

    PCI::EnumeratePCI(mcfg);
}

static void __attribute__((aligned(PAGE_SIZE))) user_main()
{
    LOOP();
}

static void setup_user()
{
    auto& pfa = PageFrameAllocator::GetKernelInstance();
    auto& ptm = PageTableManager::GetKernelInstance();

    auto stack = pfa.RequestPage();
    assert(stack && "out of memory");
    auto main = reinterpret_cast<void*>(::user_main);

    auto mapped_stack = reinterpret_cast<void*>(0xE0000000);
    auto mapped_main = reinterpret_cast<void*>(0xF0000000);

    ptm.MapPage(mapped_stack, stack, true);
    ptm.MapPage(mapped_main, main, true);

    // TODO: load user code into memory and map the used memory regions to be accessible by user mode

    jump_user(reinterpret_cast<void*>(reinterpret_cast<uptr>(mapped_stack) + PAGE_SIZE), mapped_main);
}

static void serial_exec(const string& cmd)
{
    auto args = string_view(cmd).split(',');
    for (auto& arg : args)
        arg = arg.trim();
    auto command = args.pop_front();

    if (command == "print")
    {
        for (auto& arg : args)
        {
            putn(arg.data(), arg.size());
            putc(' ');
        }
        putc('\n');
        return;
    }
    if (command == "panic")
    {
        string message;
        bool first = true;
        for (auto& arg : args)
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
    for (auto& arg : args)
    {
        Serial::Write(" '");
        Serial::Write(arg.data(), arg.size());
        Serial::Write('\'');
    }
    Serial::Write("\r\n");
}

static void test_ata()
{
    u16 bus;
    u8 device;

    if (ATA::SelectDevice(ATA_BUS_PRIMARY, ATA_DEVICE_MASTER))
    {
        bus = ATA_BUS_PRIMARY;
        device = ATA_DEVICE_MASTER;
    }
    else if (ATA::SelectDevice(ATA_BUS_PRIMARY, ATA_DEVICE_SLAVE))
    {
        bus = ATA_BUS_PRIMARY;
        device = ATA_DEVICE_SLAVE;
    }
    else if (ATA::SelectDevice(ATA_BUS_SECONDARY, ATA_DEVICE_MASTER))
    {
        bus = ATA_BUS_SECONDARY;
        device = ATA_DEVICE_MASTER;
    }
    else if (ATA::SelectDevice(ATA_BUS_SECONDARY, ATA_DEVICE_SLAVE))
    {
        bus = ATA_BUS_SECONDARY;
        device = ATA_DEVICE_SLAVE;
    }
    else
    {
        printf("failed to select ata device\n");
        return;
    }

    printf("select ata bus %04X device %02X\n", bus, device);

    if (!ATA::Identify(bus, device))
    {
        printf("failed to identify ata device\n");
        return;
    }

    u8 sector[512];
    if (!ATA::ReadSector(bus, device, 1, sizeof(sector) / 512, sector))
    {
        printf("failed to read sector from ata device\n");
        return;
    }

    for (usize j = 0; j < sizeof(sector); j += 32)
    {
        printf("%04X | ", j);
        for (usize i = 0; i < 32; ++i)
        {
            auto c = sector[j + i];
            printf("%02X ", c);
        }
        puts("| ");
        for (usize i = 0; i < 32; ++i)
        {
            auto c = sector[j + i];
            printf("%c", c < 0x20 ? '.' : c);
        }
        putc('\n');
    }
}

extern "C" void kernel_main(u32 magic, const MultibootInfo& info)
{
    InitializeStdIO();

    if (Serial::Initialize())
        Serial::Write("serial communication initialized successfully.\r\n");

    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || (reinterpret_cast<uptr>(&info) & 7))
        return;

    /**
     * Get the current kernel stack address for initializing the GDT
     */
    void* kernel_stack;
    asm volatile("mov %%esp, %0" : "=g"(kernel_stack));

    GDT::Initialize(kernel_stack);
    IDT::Initialize();

    CLI();
    PIC::Remap(PIC1_OFFSET, PIC2_OFFSET);
    PIC::Disable();
    PIC::Clr_Mask(0);
    PIT::C0_outw(PIT_DIVIDER);
    STI();

    setup_memory(info);
    setup_graphics(info);
    setup_pci(info);

    // test_ata();
    (void) test_ata;

    for (;;)
    {
        printf("\rUptime: %us                                     ", PIT::TicksSinceBoot / PIT_TICKS_PER_SECOND);
        sleep(100);
    }

    setup_user();

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
            case 0x0D:
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
