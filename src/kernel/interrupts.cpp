#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/panic.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/util.hpp>

INTER void DE_Handler(interrupt_frame *frame)
{
    Panic("Division Error\n%04x:%p", frame->CS, frame->IP);
}

INTER void DB_Handler(interrupt_frame *frame)
{
    Panic("Debug\n%04x:%p", frame->CS, frame->IP);
}

INTER void NMI_Handler(interrupt_frame *frame)
{
    Panic("Non-Maskable Interrupt\n%04x:%p", frame->CS, frame->IP);
}

INTER void BP_Handler(interrupt_frame *frame)
{
    Panic("Breakpoint\n%04x:%p", frame->CS, frame->IP);
}

INTER void OF_Handler(interrupt_frame *frame)
{
    Panic("Overflow\n%04x:%p", frame->CS, frame->IP);
}

INTER void BR_Handler(interrupt_frame *frame)
{
    Panic("Bound Range Exceeded\n%04x:%p", frame->CS, frame->IP);
}

INTER void UD_Handler(interrupt_frame *frame)
{
    Panic("Invalid Opcode\n%04x:%p", frame->CS, frame->IP);
}

INTER void NM_Handler(interrupt_frame *frame)
{
    Panic("Device Not Available\n%04x:%p", frame->CS, frame->IP);
}

INTER void DF_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Double Fault\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

INTER void TS_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("Invalid TSS\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

INTER void NP_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("Segment Not Present\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

INTER void SS_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("Stack-Segment Fault\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

INTER void GP_Handler(interrupt_frame *frame, u32 code)
{
    auto e = code & 0b1;
    auto tbl = (code >> 1) & 0b11;
    auto index = (code >> 3) & 0b1111111111111;

    Panic("General Protection Fault\n%04x:%p\nexternal=%u, table=%u, index=%04x", frame->CS, frame->IP, e, tbl, index);
}

INTER void PF_Handler(interrupt_frame *frame, u32 code)
{
    auto p = code & 0b1;
    auto w = (code >> 1) & 0b1;
    auto u = (code >> 2) & 0b1;
    auto r = (code >> 3) & 0b1;
    auto i = (code >> 4) & 0b1;
    auto pk = (code >> 5) & 0b1;
    auto ss = (code >> 6) & 0b1;
    auto sgx = (code >> 15) & 0b1;

    void *addr;
    asm volatile("mov %%cr2, %0" : "=g"(addr));

    Panic("Page Fault\n%04x:%p\np=%u, w=%u, u=%u, r=%u, i=%u, pk=%u, ss=%u, sgx=%u\nvirtual address = %p", frame->CS, frame->IP, p, w, u, r, i, pk, ss, sgx, addr);
}

INTER void MF_Handler(interrupt_frame *frame)
{
    Panic("x87 Floating-Point Exception\n%04x:%p", frame->CS, frame->IP);
}

INTER void AC_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Alignment Check\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

INTER void MC_Handler(interrupt_frame *frame)
{
    Panic("Machine Check\n%04x:%p", frame->CS, frame->IP);
}

INTER void XM_XF_Handler(interrupt_frame *frame)
{
    Panic("SIMD Floating-Point Exception\n%04x:%p", frame->CS, frame->IP);
}

INTER void VE_Handler(interrupt_frame *frame)
{
    Panic("Virtualization Exception\n%04x:%p", frame->CS, frame->IP);
}

INTER void CP_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Control Protection Exception\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

INTER void HV_Handler(interrupt_frame *frame)
{
    Panic("Hypervisor Injection Exception\n%04x:%p", frame->CS, frame->IP);
}

INTER void VC_Handler(interrupt_frame *frame, u32 code)
{
    Panic("VMM Communication Exception\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

INTER void SX_Handler(interrupt_frame *frame, u32 code)
{
    Panic("Security Exception\n%04x:%p\ncode=%u", frame->CS, frame->IP, code);
}

INTER void PIT_Handler(interrupt_frame *)
{
    static u16 counter = 0;
    static u32 timer = 0;

    counter++;

    if (counter >= 1000)
    {
        counter = 0;
        printf("\rUptime: %us", ++timer);
    }

    if (counter % 20) // 50 Hz
        Graphics::GetInstance().SwapBuffers();

    PIC::Send_EOI(0);
}

#define RET(X) asm volatile("mov %0, %%eax" : : "g"(X))

INTER void SYS_Handler(interrupt_frame *frame)
{
    auto ap = reinterpret_cast<va_list>(frame->ESP);

    auto id = va_arg(ap, u32);
    switch (id)
    {
    case 0b10000:
    {
        auto result = print(va_arg(ap, cstr));
        RET(result);
        break;
    }
    case 0b10001:
    {
        auto result = wprint(va_arg(ap, cwstr));
        RET(result);
        break;
    }
    case 0b10010:
    {
        auto string = va_arg(ap, cstr);
        auto count = va_arg(ap, usize);
        auto result = printn(string, count);
        RET(result);
        break;
    }
    case 0b10011:
    {
        auto string = va_arg(ap, cwstr);
        auto count = va_arg(ap, usize);
        auto result = wprintn(string, count);
        RET(result);
        break;
    }
    case 0b10100:
    {
        auto string = va_arg(ap, cstr);
        auto args = va_arg(ap, va_list);
        auto result = vprintf(string, args);
        RET(result);
        break;
    }
    case 0b10101:
    {
        auto string = va_arg(ap, cwstr);
        auto args = va_arg(ap, va_list);
        auto result = wvprintf(string, args);
        RET(result);
        break;
    }

    default:
        Panic("System Call\n%04x:%p\nid=%u", frame->CS, frame->IP, id);
    }
}
