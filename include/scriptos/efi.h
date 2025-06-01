#pragma once

#ifndef EFI_H
#define EFI_H

#include <scriptos/types.h>

typedef u64 EFI_PHYSICAL_ADDRESS;

typedef struct
{
    u64 Signature;
    u32 Revision;
    u32 HeaderSize;
    u32 CRC32;
    u32 Reserved;
} __attribute__((packed)) EFI_TABLE_HEADER;

typedef struct
{
    EFI_TABLE_HEADER Header;

    void* FirmwareVendor;
    u32 FirmwareRevision;

    EFI_PHYSICAL_ADDRESS ConsoleInHandle;
    void* ConsoleIn;

    EFI_PHYSICAL_ADDRESS ConsoleOutHandle;
    void* ConsoleOut;

    EFI_PHYSICAL_ADDRESS StdErrHandle;
    void* StdErr;

    void* RuntimeServices;
    void* BootServices;

    u64 NumberOfTableEntries;
    void* ConfigurationTable;
} __attribute__((packed)) EFI_SYSTEM_TABLE;

#endif
