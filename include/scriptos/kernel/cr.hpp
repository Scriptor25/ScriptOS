#pragma once

#include <scriptos/std/types.hpp>

namespace CR
{
    union CR0
    {
        static CR0 R();
        static void W(CR0 value);

        CR0(u32 value);

        operator u32() const;

        struct
        {
            /** protected mode enable */
            bool PE : 1;
            /** monitor co-processor */
            bool MP : 1;
            /** x87 FPU emulation */
            bool EM : 1;
            /** task switched */
            bool TS : 1;
            /** extension type */
            bool ET : 1;
            /** numeric error */
            bool NE : 1;
            u32 : 10;
            /** write protect */
            bool WP : 1;
            u32 : 1;
            /** alignment mask */
            bool AM : 1;
            u32 : 10;
            /** not write-through */
            bool NW : 1;
            /** cache disable */
            bool CD : 1;
            /** paging */
            bool PG : 1;
        };

        u32 Value;
    };

    union CR2
    {
        static CR2 R();
        static void W(CR2 value);

        CR2(u32 value);

        operator u32() const;

        struct
        {
            /** page fault linear address */
            u32 PFLA : 32;
        };

        u32 Value;
    };

    union CR3
    {
        static CR3 R();
        static void W(CR3 value);

        CR3(u32 value);

        operator u32() const;

        struct
        {
            u32 : 3;
            /** page-level write-through */
            bool PWT : 1;
            /** page-level cache disable */
            bool PCD : 1;
            u32 : 7;
            /** page directory base register */
            u32 PDBR : 20;
        };

        u32 Value;
    };

    union CR4
    {
        static CR4 R();
        static void W(CR4 value);

        CR4(u32 value);

        operator u32() const;

        struct
        {
            /** virtual 8086 mode extension */
            bool VME : 1;
            /** protected mode virtual interrupts */
            bool PVI : 1;
            /** time stamp disable */
            bool TSD : 1;
            /** debugging extension */
            bool DE : 1;
            /** page size extension */
            bool PSE : 1;
            /** physical address extension */
            bool PAE : 1;
            /** machine check exception */
            bool MCE : 1;
            /** page global enabled */
            bool PGE : 1;
            /** performance monitoring counter enable */
            bool PCE : 1;
            /** operating system support for FXSAVE and FXRSTOR instructions */
            bool OSFXSR : 1;
            /** operating system support for unmasked SIMD floating point exceptions */
            bool OSXMMEXCPT : 1;
            /** user mode instruction prevention */
            bool UMIP : 1;
            /** 57 bit linear addresses */
            bool LA57 : 1;
            /** virtual machine extension enable */
            bool VMXE : 1;
            /** safer mode extension enable */
            bool SMXE : 1;
            u32 : 1;
            /** enable instructions RDFSBASE, RDGSBASE, WRFSBASE, WRGSBASE */
            bool FSGSBASE : 1;
            /** PCID enable */
            bool PCIDE : 1;
            /** XSAVE and processor extended state enable */
            bool OSXSAVE : 1;
            u32 : 1;
            /** supervisor mode execution protection enable */
            bool SMEP : 1;
            /** supervisor mode access prevention enable */
            bool SMAP : 1;
            /** protection key enable */
            bool PKE : 1;
            /** control flow enforcement technology */
            bool CET : 1;
            /** enable protection keys for supervisor mode pages */
            bool PKS : 1;
            u32 : 7;
        };

        u32 Value;
    };

    union CR8
    {
        static CR8 R();
        static void W(CR8 value);

        CR8(u32 value);

        operator u32() const;

        struct
        {
            /** task priority level */
            u32 TPL : 4;
        };

        u32 Value;
    };
}