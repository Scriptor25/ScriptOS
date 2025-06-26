#include "scriptos/memory.h"

#include <scriptos/ahci.h>

bool ahci::HBA_MEM::Iterator::operator==(const Iterator& iterator) const
{
    return PI == iterator.PI && Index == iterator.Index;
}

ahci::HBA_PORT* ahci::HBA_MEM::Iterator::operator*() const
{
    if (PI & (1 << Index))
        return PCR + Index;
    return nullptr;
}

ahci::HBA_MEM::Iterator& ahci::HBA_MEM::Iterator::operator++()
{
    Index++;
    return *this;
}

ahci::HBA_MEM::Iterator ahci::HBA_MEM::begin()
{
    return { PI, PCR, 0 };
}

ahci::HBA_MEM::Iterator ahci::HBA_MEM::end()
{
    return { PI, PCR, 32 };
}

void ahci::BeginCommand(HBA_PORT* port)
{
    while (port->CMD.CR)
        ;

    port->CMD.FRE = true;
    port->CMD.ST = true;
}

void ahci::EndCommand(HBA_PORT* port)
{
    port->CMD.ST = false;
    port->CMD.FRE = false;

    while (port->CMD.FR || port->CMD.CR)
        ;
}

void ahci::RebasePort(
    HBA_PORT* port,
    uptr base_address,
    unsigned index)
{
    EndCommand(port);

    port->CLB = base_address + (index << 10);
    port->FB = base_address + (32 << 10) + (index << 8);

    memory::Fill(reinterpret_cast<void*>(port->CLB), 0, 0x400);
    memory::Fill(reinterpret_cast<void*>(port->FB), 0, 0x100);

    auto cmd_list = reinterpret_cast<HBA_CMD_HEADER*>(port->CLB);
    for (unsigned i = 0; i < 32; ++i)
    {
        auto& cmd = cmd_list[0];

        cmd.PRDTL = 8;
        cmd.CTBA = base_address + (40 << 10) + (index << 13) + (i << 8);

        memory::Fill(reinterpret_cast<void*>(cmd.CTBA), 0, 0x100);
    }

    BeginCommand(port);
}

int ahci::FindCommandSlot(HBA_PORT* port)
{
    auto slots = port->SACT | port->CI;
    for (unsigned i = 0; i < 32; ++i)
    {
        if ((slots >> i) & 1)
            continue;
        return i;
    }
    return -1;
}

bool ahci::Read(
    HBA_PORT* port,
    uptr offset,
    u16 count,
    u16* buffer)
{
    port->IS.Value = -1;

    auto slot = FindCommandSlot(port);
    if (slot < 0)
        return false;

    auto cmd_hdr = reinterpret_cast<HBA_CMD_HEADER*>(port->CLB) + slot;
    cmd_hdr->CFL = sizeof(FIS_REG_H2D) / sizeof(u32);
    cmd_hdr->W = false;
    cmd_hdr->PRDTL = ((count - 1) >> 4) + 1;

    auto cmd_tbl = reinterpret_cast<HBA_CMD_TBL*>(cmd_hdr->CTBA);
    memory::Fill(cmd_tbl, 0, sizeof(HBA_CMD_TBL) + (cmd_hdr->PRDTL - 1) * sizeof(HBA_PRDT));

    unsigned i;
    for (i = 0; i < cmd_hdr->PRDTL - 1; ++i)
    {
        auto& entry = cmd_tbl->PRDT[i];

        entry.DBA = reinterpret_cast<uptr>(buffer);
        entry.DBC = 0x1FFF;
        entry.I = 1;

        buffer += 0x1000;
        count -= 0x10;
    }

    {
        auto& entry = cmd_tbl->PRDT[i];

        entry.DBA = reinterpret_cast<uptr>(buffer);
        entry.DBC = (count << 9) - 1;
        entry.I = 1;
    }

    auto cmd_fis = reinterpret_cast<FIS_REG_H2D*>(cmd_tbl->CFIS);

    cmd_fis->FIS_Type = FIS_TYPE_REG_H2D;
    cmd_fis->C = 1;
    cmd_fis->CMD = ATA_CMD_READ_DMA_EX;

    cmd_fis->LBA0 = offset & 0xFF;
    cmd_fis->LBA1 = (offset >> 0x08) & 0xFF;
    cmd_fis->LBA2 = (offset >> 0x10) & 0xFF;
    cmd_fis->LBA3 = (offset >> 0x18) & 0xFF;
    cmd_fis->LBA4 = (offset >> 0x20) & 0xFF;
    cmd_fis->LBA5 = (offset >> 0x28) & 0xFF;

    cmd_fis->DEV = 1 << 6;
    cmd_fis->C = count;

    for (unsigned s = 0; port->TFD.STS_BSY || port->TFD.STS_DRQ; ++s)
        if (s >= 1000000)
            return false;

    port->CI = 1 << slot;

    while (port->CI & (1 << slot))
        if (port->IS.TFES)
            return false;

    if (port->IS.TFES)
        return false;

    return true;
}
