#pragma once

#include <scriptos/types.hpp>

struct PageDirectoryEntry_4MiB
{
    /* present bit */
    u8 Present : 1 = 0;
    /* read/write bit */
    u8 Read_Write : 1 = 0;
    /* user/supervisor bit */
    u8 User_Supervisor : 1 = 0;
    /* write-through bit */
    u8 Write_Through : 1 = 0;
    /* cache disable bit */
    u8 Cache_Disable : 1 = 0;
    /* accessed bit */
    u8 Accessed : 1 = 0;
    /* dirty bit */
    u8 Dirty : 1 = 0;
    /* page size bit (= 1) */
    u8 Page_Size : 1 = 0;
    /* global bit */
    u8 Global : 1 = 0;
    /* ignored */
    u8 _Ignored : 3 = 0;
    /* page attribute bit */
    u8 Page_Attribute : 1 = 0;
    /* address [39:32] */
    u32 Address_39_32 : 8 = 0;
    /* reserved */
    u8 _Reserved : 1 = 0;
    /* address [31:22] */
    u32 Address_31_22 : 10 = 0;
};

struct PageDirectoryEntry_4KiB
{
    /* present bit */
    u8 Present : 1 = 0;
    /* read/write bit */
    u8 Read_Write : 1 = 0;
    /* user/supervisor bit */
    u8 User_Supervisor : 1 = 0;
    /* write-through bit */
    u8 Write_Through : 1 = 0;
    /* cache disable bit */
    u8 Cache_Disable : 1 = 0;
    /* accessed bit */
    u8 Accessed : 1 = 0;
    /* ignored */
    u8 _Ignored_0 : 1 = 0;
    /* page size bit (= 0) */
    u8 Page_Size : 1 = 0;
    /* ignored */
    u8 _Ignored_1 : 4 = 0;
    /* address [31:12] */
    u32 Address_31_12 : 20 = 0;
};

struct PageTableEntry
{
    /* present bit */
    u8 Present : 1 = 0;
    /* read/write bit */
    u8 Read_Write : 1 = 0;
    /* user/supervisor bit */
    u8 User_Supervisor : 1 = 0;
    /* write-through bit */
    u8 Write_Through : 1 = 0;
    /* cache disable bit */
    u8 Cache_Disable : 1 = 0;
    /* accessed bit */
    u8 Accessed : 1 = 0;
    /* dirty bit */
    u8 Dirty : 1 = 0;
    /* page attribute bit */
    u8 Page_Attribute : 1 = 0;
    /* global bit */
    u8 Global : 1 = 0;
    /* ignored */
    u8 _Ignored : 3 = 0;
    /* address [31:12] */
    u32 Address_31_12 : 20 = 0;
};

union PageEntry
{
    PageDirectoryEntry_4KiB PDE;
    PageDirectoryEntry_4MiB HPDE;
    PageTableEntry PTE;
    u32 Data = 0;
};

void reset_paging();
void enable_paging();
void print_cr3();
