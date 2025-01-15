#pragma once

#include <scriptos/std/types.hpp>

extern u32 __kernel_start;
extern u32 __text_start;
extern u32 __text_end;
extern u32 __rodata_start;
extern u32 __rodata_end;
extern u32 __data_start;
extern u32 __data_end;
extern u32 __bss_start;
extern u32 __tss_start;
extern u32 __tss_end;
extern u32 __bss_end;
extern u32 __kernel_end;

#define KERNEL_START (&__kernel_start)
#define TEXT_START (&__text_start)
#define TEXT_END (&__text_end)
#define RODATA_START (&__rodata_start)
#define RODATA_END (&__rodata_end)
#define DATA_START (&__data_start)
#define DATA_END (&__data_end)
#define BSS_START (&__bss_start)
#define TSS_START (&__tss_start)
#define TSS_END (&__tss_end)
#define BSS_END (&__bss_end)
#define KERNEL_END (&__kernel_end)
