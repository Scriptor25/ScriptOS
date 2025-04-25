#pragma once

#include <scriptos/std/types.hpp>

#define ATA_BUS_PRIMARY   0x1F0
#define ATA_BUS_SECONDARY 0x170

#define ATA_DEVICE_MASTER 0x00
#define ATA_DEVICE_SLAVE  0x10

#define ATA_OFFSET_DATA           0x00
#define ATA_OFFSET_ERROR          0x01
#define ATA_OFFSET_SECTOR_COUNT   0x02
#define ATA_OFFSET_LBA_LOW        0x03
#define ATA_OFFSET_LBA_MID        0x04
#define ATA_OFFSET_LBA_HIGH       0x05
#define ATA_OFFSET_DRIVE_HEAD     0x06
#define ATA_OFFSET_COMMAND_STATUS 0x07

#define ATA_PRIMARY_DATA           (ATA_BUS_PRIMARY | ATA_OFFSET_DATA)
#define ATA_PRIMARY_ERROR          (ATA_BUS_PRIMARY | ATA_OFFSET_ERROR)
#define ATA_PRIMARY_SECTOR_COUNT   (ATA_BUS_PRIMARY | ATA_OFFSET_SECTOR_COUNT)
#define ATA_PRIMARY_LBA_LOW        (ATA_BUS_PRIMARY | ATA_OFFSET_LBA_LOW)
#define ATA_PRIMARY_LBA_MID        (ATA_BUS_PRIMARY | ATA_OFFSET_LBA_MID)
#define ATA_PRIMARY_LBA_HIGH       (ATA_BUS_PRIMARY | ATA_OFFSET_LBA_HIGH)
#define ATA_PRIMARY_DRIVE_HEAD     (ATA_BUS_PRIMARY | ATA_OFFSET_DRIVE_HEAD)
#define ATA_PRIMARY_COMMAND_STATUS (ATA_BUS_PRIMARY | ATA_OFFSET_COMMAND_STATUS)

#define ATA_SECONDARY_DATA           (ATA_BUS_SECONDARY | ATA_OFFSET_DATA)
#define ATA_SECONDARY_ERROR          (ATA_BUS_SECONDARY | ATA_OFFSET_ERROR)
#define ATA_SECONDARY_SECTOR_COUNT   (ATA_BUS_SECONDARY | ATA_OFFSET_SECTOR_COUNT)
#define ATA_SECONDARY_LBA_LOW        (ATA_BUS_SECONDARY | ATA_OFFSET_LBA_LOW)
#define ATA_SECONDARY_LBA_MID        (ATA_BUS_SECONDARY | ATA_OFFSET_LBA_MID)
#define ATA_SECONDARY_LBA_HIGH       (ATA_BUS_SECONDARY | ATA_OFFSET_LBA_HIGH)
#define ATA_SECONDARY_DRIVE_HEAD     (ATA_BUS_SECONDARY | ATA_OFFSET_DRIVE_HEAD)
#define ATA_SECONDARY_COMMAND_STATUS (ATA_BUS_SECONDARY | ATA_OFFSET_COMMAND_STATUS)

#define ATA_READ_SECTORS              0x20
#define ATA_WRITE_SECTORS             0x30
#define ATA_EXECUTE_DEVICE_DIAGNOSTIC 0x90
#define ATA_IDENTIFY                  0xEC

namespace ATA
{
    /**
     * Select ATA device for specified bus. Return true on success.
     */
    bool SelectDevice(u16 bus, u8 device);

    /**
     * Wait for selected ATA device to not be busy. Return true on success.
     */
    bool WaitBSY(u16 bus);
    /**
     * Wait for selected ATA device to be ready for commands. Return true on success.
     */
    bool WaitDRQ(u16 bus);

    /**
     * Wait for selected ATA device to not be busy and be ready for commands. Return true on success.
     */
    bool Wait(u16 bus);

    /**
     * Check if the specified ATA bus is floating, i.e. no devices are connected to it.
     */
    bool CheckFloatingBus(u16 bus);

    /**
     * Identify connected ATA device. Return true if a device is present.
     */
    bool Identify(u16 bus, u8 device);

    /**
     * Read a LBA addressed sector from selected ATA device. Buffer has to be at least of size 512 bytes. Return true on
     * success.
     */
    bool ReadSector(u16 bus, u8 device, u32 lba, u8 sector_count, void* buffer);
}
