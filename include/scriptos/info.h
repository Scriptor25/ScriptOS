#pragma once

#include <scriptos/types.h>

extern u32 _kernel_start;
extern u32 _text_start;
extern u32 _text_end;
extern u32 _rodata_start;
extern u32 _rodata_end;
extern u32 _data_start;
extern u32 _data_end;
extern u32 _bss_start;
extern u32 _bss_end;
extern u32 _kernel_end;

#define KERNEL_START _kernel_start
#define TEXT_START _text_start
#define TEXT_END _text_end
#define RODATA_START _rodata_start
#define RODATA_END _rodata_end
#define DATA_START _data_start
#define DATA_END _data_end
#define BSS_START _bss_start
#define BSS_END _bss_end
#define KERNEL_END _kernel_end
