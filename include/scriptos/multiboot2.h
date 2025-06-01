#pragma once

#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#define MULTIBOOT_SEARCH            32768
#define MULTIBOOT_HEADER_ALIGN      8
#define MULTIBOOT2_HEADER_MAGIC     0xe85250d6
#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289
#define MULTIBOOT_MOD_ALIGN         0x00001000
#define MULTIBOOT_INFO_ALIGN        0x00000008

#define MULTIBOOT_TAG_ALIGN                 8
#define MULTIBOOT_TAG_TYPE_END              0
#define MULTIBOOT_TAG_TYPE_CMDLINE          1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME 2
#define MULTIBOOT_TAG_TYPE_MODULE           3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO    4
#define MULTIBOOT_TAG_TYPE_BOOTDEV          5
#define MULTIBOOT_TAG_TYPE_MMAP             6
#define MULTIBOOT_TAG_TYPE_VBE              7
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER      8
#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS     9
#define MULTIBOOT_TAG_TYPE_APM              10
#define MULTIBOOT_TAG_TYPE_EFI32            11
#define MULTIBOOT_TAG_TYPE_EFI64            12
#define MULTIBOOT_TAG_TYPE_SMBIOS           13
#define MULTIBOOT_TAG_TYPE_ACPI_OLD         14
#define MULTIBOOT_TAG_TYPE_ACPI_NEW         15
#define MULTIBOOT_TAG_TYPE_NETWORK          16
#define MULTIBOOT_TAG_TYPE_EFI_MMAP         17
#define MULTIBOOT_TAG_TYPE_EFI_BS           18
#define MULTIBOOT_TAG_TYPE_EFI32_IH         19
#define MULTIBOOT_TAG_TYPE_EFI64_IH         20
#define MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR   21

#define MULTIBOOT_HEADER_TAG_END                 0
#define MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST 1
#define MULTIBOOT_HEADER_TAG_ADDRESS             2
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS       3
#define MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS       4
#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER         5
#define MULTIBOOT_HEADER_TAG_MODULE_ALIGN        6
#define MULTIBOOT_HEADER_TAG_EFI_BS              7
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32 8
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64 9
#define MULTIBOOT_HEADER_TAG_RELOCATABLE         10

#define MULTIBOOT_ARCHITECTURE_I386   0
#define MULTIBOOT_ARCHITECTURE_MIPS32 4
#define MULTIBOOT_HEADER_TAG_OPTIONAL 1

#define MULTIBOOT_LOAD_PREFERENCE_NONE 0
#define MULTIBOOT_LOAD_PREFERENCE_LOW  1
#define MULTIBOOT_LOAD_PREFERENCE_HIGH 2

#define MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED   1
#define MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2

#define MULTIBOOT_MEMORY_AVAILABLE        1
#define MULTIBOOT_MEMORY_RESERVED         2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS              4
#define MULTIBOOT_MEMORY_BADRAM           5

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED  0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB      1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2

#ifndef ASM_FILE

#include <scriptos/types.h>

typedef struct
{
    u32 magic;
    u32 architecture;
    u32 header_length;
    u32 checksum;
} multiboot_header;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
} multiboot_header_tag;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
    u32 requests[0];
} multiboot_header_tag_information_request;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
    u32 header_addr;
    u32 load_addr;
    u32 load_end_addr;
    u32 bss_end_addr;
} multiboot_header_tag_address;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
    u32 entry_addr;
} multiboot_header_tag_entry_address;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
    u32 console_flags;
} multiboot_header_tag_console_flags;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
    u32 width;
    u32 height;
    u32 depth;
} multiboot_header_tag_framebuffer;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
} multiboot_header_tag_module_align;

typedef struct
{
    u16 type;
    u16 flags;
    u32 size;
    u32 min_addr;
    u32 max_addr;
    u32 align;
    u32 preference;
} multiboot_header_tag_relocatable;

typedef struct
{
    u8 red;
    u8 green;
    u8 blue;
} multiboot_color;

typedef struct
{
    u64 base_addr;
    u64 length;
    u32 type;
    u32 reserved;
} multiboot_mmap_entry;

typedef struct
{
    u32 type;
    u32 size;
} multiboot_tag;

typedef struct
{
    u32 type;
    u32 size;

    char string[0];
} multiboot_tag_string;

typedef struct
{
    u32 type;
    u32 size;

    u32 mod_start;
    u32 mod_end;
    char string[0];
} multiboot_tag_module;

typedef struct
{
    u32 type;
    u32 size;

    u32 mem_lower;
    u32 mem_upper;
} multiboot_tag_basic_meminfo;

typedef struct
{
    u32 type;
    u32 size;

    u32 biosdev;
    u32 slice;
    u32 part;
} multiboot_tag_bootdev;

typedef struct
{
    u32 type;
    u32 size;

    u32 entry_size;
    u32 entry_version;
    multiboot_mmap_entry entries[0];
} multiboot_tag_mmap;

typedef struct
{
    u32 type;
    u32 size;

    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;

    u8 vbe_control_info[512];
    u8 vbe_mode_info[256];
} multiboot_tag_vbe;

typedef struct
{
    u32 type;
    u32 size;

    u64 framebuffer_addr;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    u8 reserved;

    union
    {
        struct
        {
            u32 framebuffer_palette_num_colors;
            multiboot_color framebuffer_palette[];
        };
        struct
        {
            u8 framebuffer_red_field_position;
            u8 framebuffer_red_mask_size;
            u8 framebuffer_green_field_position;
            u8 framebuffer_green_mask_size;
            u8 framebuffer_blue_field_position;
            u8 framebuffer_blue_mask_size;
        };
    };
} multiboot_tag_framebuffer;

typedef struct
{
    u32 type;
    u32 size;

    u16 num;
    u16 entsize;
    u16 shndx;
    u16 reserved;
    char sections[0];
} multiboot_tag_elf_sections;

typedef struct
{
    u32 type;
    u32 size;

    u16 version;
    u16 cseg;
    u32 offset;
    u16 cseg_16;
    u16 dseg;
    u16 flags;
    u16 cseg_len;
    u16 cseg_16_len;
    u16 dseg_len;
} multiboot_tag_apm;

typedef struct
{
    u32 type;
    u32 size;

    u32 pointer;
} multiboot_tag_efi32;

typedef struct
{
    u32 type;
    u32 size;

    u64 pointer;
} multiboot_tag_efi64;

typedef struct
{
    u32 type;
    u32 size;

    u8 major;
    u8 minor;
    u8 reserved[6];
    u8 tables[0];
} multiboot_tag_smbios;

typedef struct
{
    u32 type;
    u32 size;

    u8 rsdp[0];
} multiboot_tag_old_acpi;

typedef struct
{
    u32 type;
    u32 size;

    u8 rsdp[0];
} multiboot_tag_new_acpi;

typedef struct
{
    u32 type;
    u32 size;

    cstr dhcpack;
} multiboot_tag_network;

typedef struct
{
    u32 type;
    u32 size;

    u32 descr_size;
    u32 descr_vers;
    u8 efi_mmap[0];
} multiboot_tag_efi_mmap;

typedef struct
{
    u32 type;
    u32 size;

    u32 pointer;
} multiboot_tag_efi32_ih;

typedef struct
{
    u32 type;
    u32 size;

    u64 pointer;
} multiboot_tag_efi64_ih;

typedef struct
{
    u32 type;
    u32 size;

    u32 load_base_addr;
} multiboot_tag_load_base_addr;

typedef struct
{
    u32 total_size;
    u32 reserved;
} multiboot_info;

#endif
#endif
