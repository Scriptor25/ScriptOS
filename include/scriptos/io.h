#pragma once

#include <scriptos/types.h>

namespace io
{
    inline void outb(
        u16 port,
        u8 data)
    {
        asm volatile("outb %0, %1" ::"a"(data), "Nd"(port));
    }

    inline void outw(
        u16 port,
        u16 data)
    {
        asm volatile("outw %0, %1" ::"a"(data), "Nd"(port));
    }

    inline void outl(
        u16 port,
        u32 data)
    {
        asm volatile("outl %0, %1" ::"a"(data), "Nd"(port));
    }

    inline u8 inb(u16 port)
    {
        u8 data;
        asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

    inline u16 inw(u16 port)
    {
        u16 data;
        asm volatile("inw %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

    inline u32 inl(u16 port)
    {
        u32 data;
        asm volatile("inl %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

    inline void outsb(
        u16 port,
        const u8* buffer,
        usize count)
    {
        asm volatile("rep outsb" : "+S"(buffer), "+c"(count) : "d"(port));
    }

    inline void outsw(
        u16 port,
        const u16* buffer,
        usize count)
    {
        asm volatile("rep outsw" : "+S"(buffer), "+c"(count) : "d"(port));
    }

    inline void outsl(
        u16 port,
        const u32* buffer,
        usize count)
    {
        asm volatile("rep outsl" : "+S"(buffer), "+c"(count) : "d"(port));
    }

    inline u8* insb(
        u16 port,
        u8* buffer,
        usize count)
    {
        asm volatile("rep insb" : "+D"(buffer), "+c"(count) : "d"(port));
        return buffer;
    }

    inline u16* insw(
        u16 port,
        u16* buffer,
        usize count)
    {
        asm volatile("rep insw" : "+D"(buffer), "+c"(count) : "d"(port));
        return buffer;
    }

    inline u32* insl(
        u16 port,
        u32* buffer,
        usize count)
    {
        asm volatile("rep insl" : "+D"(buffer), "+c"(count) : "d"(port));
        return buffer;
    }

    inline void wait()
    {
        outb(0x0080, 0x00);
    }
}
