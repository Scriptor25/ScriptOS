#pragma once

#include <scriptos/std/types.hpp>

/**
 * at kernel start
 */
extern u32 __kernel_start;
/**
 * at text start
 */
extern u32 __text_start;
/**
 * at user text start
 */
extern u32 __user_text_start;
/**
 * at user text end
 */
extern u32 __user_text_end;
/**
 * at text end
 */
extern u32 __text_end;
/**
 * at read-only data start
 */
extern u32 __rodata_start;
/**
 * at read-only data end
 */
extern u32 __rodata_end;
/**
 * at data start
 */
extern u32 __data_start;
/**
 * at data end
 */
extern u32 __data_end;
/**
 * at bss start
 */
extern u32 __bss_start;
/**
 * at bss end
 */
extern u32 __bss_end;
/**
 * at kernel end
 */
extern u32 __kernel_end;

/**
 * kernel start address
 */
#define KERNEL_START (reinterpret_cast<void *>(&__kernel_start))
/**
 * text start address
 */
#define TEXT_START (reinterpret_cast<void *>(&__text_start))
/**
 * user text start address
 */
#define USER_TEXT_START (reinterpret_cast<void *>(&__user_text_start))
/**
 * user text end address
 */
#define USER_TEXT_END (reinterpret_cast<void *>(&__user_text_end))
/**
 * text end address
 */
#define TEXT_END (reinterpret_cast<void *>(&__text_end))
/**
 * read-only data start address
 */
#define RODATA_START (reinterpret_cast<void *>(&__rodata_start))
/**
 * read-only data end address
 */
#define RODATA_END (reinterpret_cast<void *>(&__rodata_end))
/**
 * data start address
 */
#define DATA_START (reinterpret_cast<void *>(&__data_start))
/**
 * data end address
 */
#define DATA_END (reinterpret_cast<void *>(&__data_end))
/**
 * bss start address
 */
#define BSS_START (reinterpret_cast<void *>(&__bss_start))
/**
 * bss end address
 */
#define BSS_END (reinterpret_cast<void *>(&__bss_end))
/**
 * kernel end address
 */
#define KERNEL_END (reinterpret_cast<void *>(&__kernel_end))

/**
 * kernel size
 */
#define KERNEL_SIZE (reinterpret_cast<uptr>(KERNEL_END) - reinterpret_cast<uptr>(KERNEL_START))
/**
 * text size
 */
#define TEXT_SIZE (reinterpret_cast<uptr>(TEXT_END) - reinterpret_cast<uptr>(TEXT_START))
/**
 * user text size
 */
#define USER_TEXT_SIZE (reinterpret_cast<uptr>(USER_TEXT_END) - reinterpret_cast<uptr>(USER_TEXT_START))
/**
 * read-only data size
 */
#define RODATA_SIZE (reinterpret_cast<uptr>(RODATA_END) - reinterpret_cast<uptr>(RODATA_START))
/**
 * data size
 */
#define DATA_SIZE (reinterpret_cast<uptr>(DATA_END) - reinterpret_cast<uptr>(DATA_START))
/**
 * bss size
 */
#define BSS_SIZE (reinterpret_cast<uptr>(BSS_END) - reinterpret_cast<uptr>(BSS_START))
