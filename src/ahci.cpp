#include <scriptos/ahci.h>
#include <scriptos/ahci/fis.h>
#include <scriptos/ahci/hba.h>
#include <scriptos/ata.h>
#include <scriptos/atapi.h>
#include <scriptos/memory.h>

void ahci::StartCommand(hba::PortT* port)
{
    while (port->Command_Status.CommandListRunning)
        ;

    port->Command_Status.Start = true;
}

void ahci::StopCommand(hba::PortT* port)
{
    port->Command_Status.Start = false;
    while (port->Command_Status.CommandListRunning)
        ; // wait at least 500ms, then try reset

    port->Command_Status.FISReceiveEnable = false;
    while (port->Command_Status.FISReceiveRunning)
        ; // wait at least 500ms, then try reset
}

bool ahci::Initialize(
    hba::MEM_T* abar,
    hba::PortT* port,
    uptr base_address)
{
    if (base_address & 0x7F)
        return false;

    StopCommand(port);

    const auto slot_count = abar->CAP.NCS;

    port->CommandListBase = base_address;
    memory::Fill(reinterpret_cast<void*>(port->CommandListBase), 0, 0x400);

    const auto command_list = reinterpret_cast<hba::CommandHeaderT*>(port->CommandListBase);

    port->FISBase = base_address + (32 << 10);
    memory::Fill(reinterpret_cast<void*>(port->FISBase), 0, 0x100);

    port->Command_Status.FISReceiveEnable = true;
    *reinterpret_cast<u32*>(&port->SATAError) = 0xFFFFFFFF;

    for (unsigned slot = 0; slot < slot_count; ++slot)
    {
        const auto command_header = command_list + slot;

        command_header->PRDTableLength = 8;
        command_header->CommandTableBase = base_address + (40 << 10) + (slot << 8);

        memory::Fill(reinterpret_cast<void*>(command_header->CommandTableBase), 0, 0x100);
    }

    StartCommand(port);
    return true;
}

int ahci::FindSlot(
    hba::MEM_T* abar,
    hba::PortT* port)
{
    const auto slot_count = abar->CAP.NCS;
    const auto slots = port->SATAActive | port->CommandIssue;

    for (unsigned i = 0; i < slot_count; ++i)
        if (!((slots >> i) & 1))
            return i;

    return -1;
}

bool ahci::SendATAPICommand(
    hba::MEM_T* abar,
    hba::PortT* port,
    const u8* command,
    usize command_length,
    void* buffer,
    usize buffer_length)
{
    for (unsigned timeout = 1000000;
         port->TaskFileData.Status.Busy || port->TaskFileData.Status.DataTransferRequest;
         --timeout)
        if (!timeout)
            return false;

    auto slot = FindSlot(abar, port);
    if (slot < 0)
        return false;

    const auto command_header = reinterpret_cast<hba::CommandHeaderT*>(port->CommandListBase) + slot;
    command_header->ATAPI = true;
    command_header->Write = false;
    command_header->Prefetchable = false;
    command_header->PRDTableLength = 1;
    command_header->Command_FIS_Length = sizeof(fis::HostToDeviceT) / sizeof(u32);

    const auto command_table = reinterpret_cast<hba::CommandTableT*>(
        command_header->CommandTableBase);
    memory::Fill(command_table, 0, sizeof(hba::CommandTableT));

    command_table->PRDTable->DataBaseAddress = reinterpret_cast<uptr>(buffer);
    command_table->PRDTable->DataByteCount = buffer_length - 1;
    command_table->PRDTable->Interrupt = true;

    memory::Copy(command_table->ATAPI_Command, command, command_length);

    const auto command_fis = reinterpret_cast<fis::HostToDeviceT*>(command_table->Command_FIS);
    memory::Fill(command_fis, 0, sizeof(fis::HostToDeviceT));

    command_fis->Type = fis::Type_HostToDevice;
    command_fis->IsCommand = true;
    command_fis->Command = ata::Command_PACKET;
    command_fis->Count = command_length / sizeof(u32);

    *reinterpret_cast<u32*>(&port->InterruptStatus) = 0xFFFFFFFF;

    const auto mask = 1 << slot;
    port->CommandIssue = mask;

    for (unsigned timeout = 1000000; port->CommandIssue & mask; --timeout)
        if (port->InterruptStatus.TaskFileErrorStatus || !timeout)
            return false;

    return true;
}

bool ahci::ReadATA(
    hba::MEM_T* abar,
    hba::PortT* port,
    uptr lba,
    u16 sector_count,
    void* buffer)
{
    for (unsigned timeout = 1000000;
         port->TaskFileData.Status.Busy || port->TaskFileData.Status.DataTransferRequest;
         --timeout)
        if (!timeout)
            return false;

    auto slot = FindSlot(abar, port);
    if (slot < 0)
        return false;

    const auto command_header = reinterpret_cast<hba::CommandHeaderT*>(port->CommandListBase) + slot;
    command_header->ATAPI = false;
    command_header->Write = false;
    command_header->Prefetchable = false;
    command_header->PRDTableLength = ((sector_count - 1) >> 4) + 1;
    command_header->Command_FIS_Length = sizeof(fis::HostToDeviceT) / sizeof(u32);

    const auto command_table = reinterpret_cast<hba::CommandTableT*>(
        command_header->CommandTableBase);
    memory::Fill(command_table, 0, sizeof(hba::CommandTableT) + (command_header->PRDTableLength - 1) * sizeof(hba::PRDTableEntryT));

    auto data_base_address = reinterpret_cast<uptr>(buffer);

    unsigned i;
    for (i = 0; i < command_header->PRDTableLength - 1u; ++i)
    {
        const auto entry = command_table->PRDTable + i;

        entry->DataBaseAddress = data_base_address;
        entry->DataByteCount = 0x1FFF;
        entry->Interrupt = true;

        data_base_address += 0x2000;
        sector_count -= 0x10;
    }
    {
        const auto entry = command_table->PRDTable + i;

        entry->DataBaseAddress = data_base_address;
        entry->DataByteCount = (sector_count << 9) - 1;
        entry->Interrupt = true;
    }

    const auto command_fis = reinterpret_cast<fis::HostToDeviceT*>(command_table->Command_FIS);
    command_fis->Type = fis::Type_HostToDevice;
    command_fis->IsCommand = true;
    command_fis->Command = ata::Command_READ_DMA_EXT;
    command_fis->Device = 1 << 6;
    command_fis->Count = sector_count;
    command_fis->LBA0 = (lba >> 0x00) & 0xFF;
    command_fis->LBA1 = (lba >> 0x08) & 0xFF;
    command_fis->LBA2 = (lba >> 0x10) & 0xFF;
    command_fis->LBA3 = (lba >> 0x18) & 0xFF;
    command_fis->LBA4 = (lba >> 0x20) & 0xFF;
    command_fis->LBA5 = (lba >> 0x28) & 0xFF;

    *reinterpret_cast<u32*>(&port->InterruptStatus) = 0xFFFFFFFF;

    const auto mask = 1 << slot;
    port->CommandIssue = mask;

    for (unsigned timeout = 1000000; port->CommandIssue & mask; --timeout)
        if (port->InterruptStatus.TaskFileErrorStatus || !timeout)
            return false;

    return true;
}

bool ahci::ReadATAPI(
    hba::MEM_T* abar,
    hba::PortT* port,
    uptr lba,
    u16 sector_count,
    void* buffer)
{
    const u8 command[12]{
        atapi::Command_READ_10,
        0x00,
        static_cast<u8>((lba >> 0x18) & 0xFF),
        static_cast<u8>((lba >> 0x10) & 0xFF),
        static_cast<u8>((lba >> 0x08) & 0xFF),
        static_cast<u8>((lba >> 0x00) & 0xFF),
        0x00, // (sector_count >> 0x18) & 0xF
        0x00, // (sector_count >> 0x10) & 0xF
        static_cast<u8>((sector_count >> 0x08) & 0xFF),
        static_cast<u8>((sector_count >> 0x00) & 0xFF),
        0x00,
        0x00,
    };

    return SendATAPICommand(abar, port, command, sizeof(command), buffer, sector_count << 11);
}
