#include <scriptos/kernel/ata.hpp>
#include <scriptos/kernel/io.hpp>
#include <scriptos/std/debug.hpp>
#include <scriptos/std/print.hpp>

bool ATA::SelectDevice(u16 bus, u8 device)
{
    outb(bus | ATA_OFFSET_DRIVE_HEAD, 0xE0 | device);
    for (usize i = 0; i < 15; ++i)
        inb(bus | ATA_OFFSET_COMMAND_STATUS);

    return !CheckFloatingBus(bus);
}

bool ATA::WaitBSY(u16 bus)
{
    for (usize i = 0; i < 4; ++i)
        inb(bus | ATA_OFFSET_COMMAND_STATUS);
    for (usize i = 0; i < 100000; ++i)
    {
        auto status = inb(bus | ATA_OFFSET_COMMAND_STATUS);
        if (!(status & 0x80))
            return true;
        if (status & 0x01)
            return false;
    }
    return false;
}

bool ATA::WaitDRQ(u16 bus)
{
    for (usize i = 0; i < 4; ++i)
        inb(bus | ATA_OFFSET_COMMAND_STATUS);
    for (usize i = 0; i < 100000; ++i)
    {
        auto status = inb(bus | ATA_OFFSET_COMMAND_STATUS);
        if (status & 0x08)
            return true;
        if (status & 0x01)
            return false;
    }
    return false;
}

bool ATA::Wait(u16 bus)
{
    return WaitBSY(bus) && WaitDRQ(bus);
}

bool ATA::CheckFloatingBus(u16 bus)
{
    auto status = inb(bus | ATA_OFFSET_COMMAND_STATUS);
    return (status == 0xFF || status == 0x00);
}

bool ATA::Identify(u16 bus, u8 device)
{
    outb(bus | ATA_OFFSET_DRIVE_HEAD, 0xE0 | device);
    outb(bus | ATA_OFFSET_SECTOR_COUNT, 0);
    outb(bus | ATA_OFFSET_LBA_LOW, 0);
    outb(bus | ATA_OFFSET_LBA_MID, 0);
    outb(bus | ATA_OFFSET_LBA_HIGH, 0);
    outb(bus | ATA_OFFSET_COMMAND_STATUS, ATA_IDENTIFY);

    if (CheckFloatingBus(bus))
        return false;

    if (!WaitBSY(bus))
        return false;

    auto lba_mid = inb(bus | ATA_OFFSET_LBA_MID);
    auto lba_high = inb(bus | ATA_OFFSET_LBA_HIGH);
    if (lba_mid != 0 || lba_high != 0)
        return false;

    if (!WaitDRQ(bus))
        return false;

    u8 sector[512];
    insw(bus | ATA_OFFSET_DATA, sector, 256);

    // TODO: do something with the identifier sector

    return true;
}

bool ATA::ReadSector(u16 bus, u8 device, u32 lba, u8 sector_count, void* buffer)
{
    if (!WaitBSY(bus))
        return false;

    outb(bus | ATA_OFFSET_DRIVE_HEAD, 0xE0 | device | ((lba >> 24) & 0x0F));
    outb(bus | ATA_OFFSET_SECTOR_COUNT, sector_count);
    outb(bus | ATA_OFFSET_LBA_LOW, static_cast<u8>(lba & 0xFF));
    outb(bus | ATA_OFFSET_LBA_MID, static_cast<u8>((lba >> 8) & 0xFF));
    outb(bus | ATA_OFFSET_LBA_HIGH, static_cast<u8>((lba >> 16) & 0xFF));
    outb(bus | ATA_OFFSET_COMMAND_STATUS, ATA_READ_SECTORS);

    if (!Wait(bus))
        return false;

    insw(bus | ATA_OFFSET_DATA, buffer, sector_count * 0x100);
    return true;
}
