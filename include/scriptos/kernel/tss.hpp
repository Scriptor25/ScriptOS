#pragma once

#include <scriptos/std/types.hpp>

/**
 * Task State Segment
 */
struct TSS
{
    u16 Link;
    u16 reserved1;
    u32 ESP0;
    u16 SS0;
    u16 reserved2;
    u32 ESP1;
    u16 SS1;
    u16 reserved3;
    u32 ESP2;
    u16 SS2;
    u16 reserved4;
    u32 CR3;
    u32 EIP;
    u32 EFLAGS;
    u32 EAX;
    u32 ECX;
    u32 EDX;
    u32 EBX;
    u32 ESP;
    u32 EBP;
    u32 ESI;
    u32 EDI;
    u16 ES;
    u16 reserved5;
    u16 CS;
    u16 reserved6;
    u16 SS;
    u16 reserved7;
    u16 DS;
    u16 reserved8;
    u16 FS;
    u16 reserved9;
    u16 GS;
    u16 reserved10;
    u16 LDTR;
    u16 reserved11;
    u16 reserved12;
    u16 IOPB;
    u32 SSP;
};
