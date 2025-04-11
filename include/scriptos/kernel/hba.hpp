#pragma once

#include <scriptos/kernel/fis.hpp>
#include <scriptos/std/types.hpp>

namespace HBA
{
    struct Port
    {
        /** 0x00, command list base address, 1K-byte aligned */
        u32 CLB;
        /** 0x04, command list base address upper 32 bits */
        u32 CLBU;
        /** 0x08, FIS base address, 256-byte aligned */
        u32 FB;
        /** 0x0C, FIS base address upper 32 bits */
        u32 FBU;
        /** 0x10, interrupt status */
        u32 IS;
        /** 0x14, interrupt enable */
        u32 IE;
        /** 0x18, command and status */
        u32 CMD;
        /** 0x1C, Reserved */
        u32 _0;
        /** 0x20, task file data */
        u32 TFD;
        /** 0x24, signature */
        u32 SIG;
        /** 0x28, SATA status (SCR0:SStatus) */
        u32 SSTS;
        /** 0x2C, SATA control (SCR2:SControl) */
        u32 SCTL;
        /** 0x30, SATA error (SCR1:SError) */
        u32 SERR;
        /** 0x34, SATA active (SCR3:SActive) */
        u32 SACT;
        /** 0x38, command issue */
        u32 CI;
        /** 0x3C, SATA notification (SCR4:SNotification) */
        u32 SNTF;
        /** 0x40, FIS-based switch control */
        u32 FBS;
        /** 0x44 ~ 0x6F, Reserved */
        u32 _1[11];
        /** 0x70 ~ 0x7F, vendor specific */
        u32 Vendor[4];
    } __attribute__((packed));

    struct Memory
    {
        /** 0x00, Host capability */
        u32 CAP;
        /** 0x04, Global host control */
        u32 GHC;
        /** 0x08, Interrupt status */
        u32 IS;
        /** 0x0C, Port implemented */
        u32 PI;
        /** 0x10, Version */
        u32 VS;
        /** 0x14, Command completion coalescing control */
        u32 CCC_CTL;
        /** 0x18, Command completion coalescing ports */
        u32 CCC_PTS;
        /** 0x1C, Enclosure management location */
        u32 EM_LOC;
        /** 0x20, Enclosure management control */
        u32 EM_CTL;
        /** 0x24, Host capabilities extended */
        u32 CAP2;
        /** 0x28, BIOS/OS handoff control and status */
        u32 BOHC;

        /** 0x2C ~ 0x9F, Reserved */
        u8 _0[0xA0 - 0x2C];

        /** 0xA0 ~ 0xFF, Vendor specific registers */
        u8 Vendor[0x100 - 0xA0];

        /** 0x100 ~ 0x10FF, Port control registers */
        Port Ports[0];
    } __attribute__((packed));

    struct Received_FIS
    {
        /** 0x00, DMA Setup FIS */
        FIS::DMA_Setup DSFIS;
        u8 _0[4];

        /** 0x20, PIO Setup FIS */
        FIS::PIO_Setup PSFIS;
        u8 _1[12];

        /** 0x40, Register - Device to Host FIS */
        FIS::Device_To_Host RFIS;
        u8 _2[4];

        /** 0x58, Set Device Bit FIS */
        FIS::Device_Bits SDBFIS;

        /** 0x60 */
        u8 UFIS[64];

        /** 0xA0 */
        u8 _3[0x100 - 0xA0];
    } __attribute__((packed));

    struct Command_Header
    {
        /** 0x00:0, Command FIS length in DWORDS, 2 ~ 16 */
        u8 CFL : 5;
        /** 0x00:5, ATAPI */
        u8 A : 1;
        /** 0x00:6, Write, 1: H2D, 0: D2H */
        u8 W : 1;
        /** 0x00:7, Prefetchable */
        u8 P : 1;

        /** 0x01:0, Reset */
        u8 R : 1;
        /** 0x01:1, BIST */
        u8 B : 1;
        /** 0x01:2, Clear busy upon R_OK */
        u8 C : 1;
        /** 0x01:3, Reserved */
        u8 _0 : 1;
        /** 0x01:4, Port multiplier port */
        u8 PMP : 4;

        /** 0x02, Physical region descriptor table length in entries */
        u16 PRDTL;

        /** 0x04, Physical region descriptor byte count transferred */
        u32 PRDBC;

        /** 0x08, Command table descriptor base address */
        u32 CTBA;
        /** 0x0C, Command table descriptor base address upper */
        u32 CTBAU;

        /** 0x10 ~ 0x1F, Reserved */
        u8 _1[16];
    } __attribute__((packed));

    struct PRDT_Entry
    {
        /** 0x00, Data base address */
        u32 DBA;
        /** 0x04, Data base address upper 32 bits */
        u32 DBAU;
        /** 0x08, Reserved */
        u8 _0[4];

        /** 0x0C, Byte count, 4M max */
        u32 DBC : 22;
        /** 0x0E:6, Reserved */
        u32 _1 : 9;
        /** 0x0F:7, Interrupt on completion */
        u32 I : 1;
    } __attribute__((packed));

    struct Command_Table
    {
        /** 0x00 ~ 0x3F, Command FIS */
        u8 CFIS[64];

        /** 0x40 ~ 0x4F, ATAPI command, 12 or 16 bytes */
        u8 ACMD[16];

        /** 0x50 ~ 0x7F, Reserved */
        u8 _0[48];

        /** 0x80 ~ ..., Physical region descriptor table entries, 0 ~ 65535 */
        PRDT_Entry Entries[0];
    } __attribute__((packed));
}
