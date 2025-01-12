#pragma once

#include <scriptos/types.hpp>

#define NUM_ENTRIES 0x400
#define PAGE_SIZE 0x1000

struct PageDirectoryEntry_4MiB
{
    /* present bit */
    bool Present : 1;
    /* read/write bit */
    bool ReadWrite : 1;
    /* user/supervisor bit */
    bool UserSupervisor : 1;
    /* write-through bit */
    bool WriteThrough : 1;
    /* cache disable bit */
    bool CacheDisable : 1;
    /* accessed bit */
    bool Accessed : 1;
    /* dirty bit */
    bool Dirty : 1;
    /* page size bit (= 1) */
    bool PageSize : 1;
    /* global bit */
    bool Global : 1;
    /* ignored */
    u8 _Ignored : 3;
    /* page attribute bit */
    bool PageAttribute : 1;
    /* address [39:32] */
    u32 Address_39_32 : 8;
    /* reserved */
    bool _Reserved : 1;
    /* address [31:22] */
    u32 Address_31_22 : 10;
};

union PageDirectoryEntry
{
    struct
    {
        /* present bit */
        bool Present : 1;
        /* read/write bit */
        bool ReadWrite : 1;
        /* user/supervisor bit */
        bool UserSupervisor : 1;
        /* write-through bit */
        bool WriteThrough : 1;
        /* cache disable bit */
        bool CacheDisable : 1;
        /* accessed bit */
        bool Accessed : 1;
        /* ignored */
        u8 _Ignored_0 : 1;
        /* page size bit (= 0) */
        bool PageSize : 1;
        /* ignored */
        u8 _Ignored_1 : 4;
        /* address [31:12] */
        u32 Address_31_12 : 20;
    };

    u32 Raw;
};

union PageTableEntry
{
    struct
    {
        /* present bit */
        bool Present : 1;
        /* read/write bit */
        bool ReadWrite : 1;
        /* user/supervisor bit */
        bool UserSupervisor : 1;
        /* write-through bit */
        bool WriteThrough : 1;
        /* cache disable bit */
        bool CacheDisable : 1;
        /* accessed bit */
        bool Accessed : 1;
        /* dirty bit */
        bool Dirty : 1;
        /* page attribute bit */
        bool PageAttribute : 1;
        /* global bit */
        bool Global : 1;
        /* ignored */
        u8 _Ignored : 3;
        /* address [31:12] */
        u32 Address_31_12 : 20;
    };

    u32 Raw;
};
