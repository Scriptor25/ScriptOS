#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/mb_info.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/kernel/pit.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/kernel/user.hpp>
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

#define SERIAL_PORT_COM1 0x3f8 // COM1

static int Serial_Init()
{
    out<u8>(SERIAL_PORT_COM1 + 1, 0x00); // Disable all interrupts
    out<u8>(SERIAL_PORT_COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    out<u8>(SERIAL_PORT_COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    out<u8>(SERIAL_PORT_COM1 + 1, 0x00); //                  (hi byte)
    out<u8>(SERIAL_PORT_COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    out<u8>(SERIAL_PORT_COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    out<u8>(SERIAL_PORT_COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
    out<u8>(SERIAL_PORT_COM1 + 4, 0x1E); // Set in loopback mode, test the serial chip
    out<u8>(SERIAL_PORT_COM1 + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (in<u8>(SERIAL_PORT_COM1 + 0) != 0xAE)
        return 1;

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    out<u8>(SERIAL_PORT_COM1 + 4, 0x0F);
    return 0;
}

static int Serial_Received()
{
    return in<u8>(SERIAL_PORT_COM1 + 5) & 1;
}

static char Serial_Read()
{
    while (Serial_Received() == 0)
        ;

    return in<u8>(SERIAL_PORT_COM1);
}

static int Serial_Transmit_Empty()
{
    return in<u8>(SERIAL_PORT_COM1 + 5) & 0x20;
}

static void Serial_Write(char a)
{
    while (Serial_Transmit_Empty() == 0)
        ;

    out<u8>(SERIAL_PORT_COM1, a);
}

static void Serial_Write(cstr string)
{
    for (auto p = (str)string; *p; ++p)
        Serial_Write(*p);
}

static void exec(cstr cmd)
{
    (void)cmd;
}

extern "C" void kernel_main(u32 magic, const MultibootInfo &info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((uptr)&info & 7))
        return;

    setup_memory(info);
    setup_graphics(info);

    void *kernel_stack;
    asm volatile("mov %%esp, %0" : "=r"(kernel_stack) :);

    InitGDT(kernel_stack);
    InitIDT();

    CLI();
    PIC_Remap(PIC1_OFFSET, PIC2_OFFSET);
    PIC_Disable();
    PIC_Clr_Mask(0);
    PIT_Write_C0_w(1193); // 1000Hz
    STI();

    // auto user_stack = PageFrameAllocator::GetInstance().RequestPage();
    // PageTableManager::GetKernelInstance().MapPage((void *)0xC0000000, user_stack, true);

    // auto page_count = ceil_div((uptr)USER_TEXT_END - (uptr)USER_TEXT_START, PAGE_SIZE);
    // PageTableManager::GetKernelInstance().MapPages(USER_TEXT_START, USER_TEXT_START, page_count, true);

    // jump_user_main((void *)(0xC0000000 + PAGE_SIZE));

    auto error = Serial_Init();
    if (error)
    {
        printf("failed to init serial\n");
        LOOP();
    }

    Serial_Write("Hello Serial Terminal!\r\n> ");

    char buffer[0x100];
    usize index = 0;

    while (true)
    {
        auto c = Serial_Read();
        switch (c)
        {
        case 0x08:
            if (index == 0)
                break;
            index--;
            Serial_Write(0x08);
            Serial_Write(' ');
            Serial_Write(0x08);
            break;
        case 0x0d:
            buffer[index++] = 0;
            exec(buffer);
            index = 0;
            Serial_Write("\r\n> ");
            break;
        default:
            if (c >= 0x20)
            {
                buffer[index++] = c;
                Serial_Write(c);
            }
            break;
        }
    }
}
