#pragma once

#include <scriptos/ahci/hba.h>
#include <scriptos/types.h>

namespace ahci
{
    void StartCommand(hba::PortT* port);
    void StopCommand(hba::PortT* port);

    bool Initialize(
        hba::MEM_T* abar,
        hba::PortT* port,
        uptr base_address);

    int FindSlot(
        hba::MEM_T* abar,
        hba::PortT* port);

    bool SendATAPICommand(
        hba::MEM_T* abar,
        hba::PortT* port,
        const u8* command,
        usize command_length,
        void* buffer,
        usize buffer_length);

    bool ReadATA(
        hba::MEM_T* abar,
        hba::PortT* port,
        uptr lba,
        u16 sector_count,
        void* buffer);

    bool ReadATAPI(
        hba::MEM_T* abar,
        hba::PortT* port,
        uptr lba,
        u16 sector_count,
        void* buffer);
}
