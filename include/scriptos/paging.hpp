#pragma once

#include <scriptos/types.hpp>

struct PageDirectoryEntry4MiB
{
    /* present bit */
    u8 P : 1;
    /* read/write bit */
    u8 R_W : 1;
    /* user/supervisor bit */
    u8 U_S : 1;
    /* write-through bit */
    u8 PWT : 1;
    /* cache disable bit */
    u8 PCD : 1;
    /* accessed bit */
    u8 A : 1;
    /* dirty bit */
    u8 D : 1;
    /* page size bit */
    u8 PS : 1;
    /* global bit */
    u8 G : 1;
    u8 reserved0 : 3;
    /* page attribute bit */
    u8 PAT : 1;
    /* address [39:32] */
    u32 Address_39_32 : 8;
    u8 reserved1 : 1;
    /* address [31:22] */
    u32 Address_31_22 : 10;
};

struct PageDirectoryEntry4KiB
{
    /* present bit */
    u8 P : 1;
    /* read/write bit */
    u8 R_W : 1;
    /* user/supervisor bit */
    u8 U_S : 1;
    /* write-through bit */
    u8 PWT : 1;
    /* cache disable bit */
    u8 PCD : 1;
    /* accessed bit */
    u8 A : 1;
    u8 reserved0 : 1;
    /* page size bit */
    u8 PS : 1;
    u8 reserved1 : 4;
    /* address [31:12] */
    u32 Address_31_12 : 20;
};

struct PageTableEntry
{
    u8 P : 1;
    u8 R_W : 1;
    u8 U_S : 1;
    u8 PWT : 1;
    u8 PCD : 1;
    u8 A : 1;
    u8 D : 1;
    u8 PAT : 1;
    u8 G : 1;
    u8 reserved : 3;
    u32 Address_31_12 : 20;
};
