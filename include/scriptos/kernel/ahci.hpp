#pragma once

/** SATA drive */
#define SATA_SIG_ATA 0x00000101
/** SATAPI drive */
#define SATA_SIG_ATAPI 0xEB140101
/** Enclosure management bridge */
#define SATA_SIG_SEMB 0xC33C0101
/** Port multiplier */
#define SATA_SIG_PM 0x96690101

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4
