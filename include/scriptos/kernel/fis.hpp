#pragma once

#include <scriptos/std/types.hpp>

/**
 * FIS - Frame Information Structure
 */
namespace FIS
{
    enum Type
    {
        /** Register FIS - host to device */
        Type_Host_To_Device = 0x27,
        /** Register FIS - device to host */
        Type_Device_To_Host = 0x34,
        /** DMA activate FIS - device to host */
        Type_DMA_Activate = 0x39,
        /** DMA setup FIS - bidirectional */
        Type_DMA_Setup = 0x41,
        /** Data FIS - bidirectional */
        Type_Data = 0x46,
        /** BIST activate FIS - bidirectional */
        Type_BIST = 0x58,
        /** PIO setup FIS - device to host */
        Type_PIO_Setup = 0x5F,
        /** Set device bits FIS - device to host */
        Type_Device_Bits = 0xA1,
    };

    struct Host_To_Device
    {
        /** 0x00, Type_Host_To_Device */
        Type FIS_Type : 8;

        /** 0x01:0, Port multiplier */
        u8 PMPort : 4;
        /** 0x01:4, Reserved */
        u8 _0 : 3;
        /** 0x01:7, 1: Command, 0: Control */
        u8 C : 1;

        /** 0x02, Command register */
        u8 Command;
        /** 0x03, Feature register, 7:0 */
        u8 FeatureLo;

        /** 0x04, LBA low register, 7:0 */
        u8 LBA0;
        /** 0x05, LBA mid register, 15:8 */
        u8 LBA1;
        /** 0x06, LBA high register, 23:16 */
        u8 LBA2;
        /** 0x07, Device register */
        u8 Device;

        /** 0x08, LBA register, 31:24 */
        u8 LBA3;
        /** 0x09, LBA register, 39:32 */
        u8 LBA4;
        /** 0x0A, LBA register, 47:40 */
        u8 LBA5;
        /** 0x0B, Feature register, 15:8 */
        u8 FeatureHi;

        /** 0x0C, Count register, 7:0 */
        u8 CountLo;
        /** 0x0D, Count register, 15:8 */
        u8 CountHi;
        /** 0x0E, Isochronous command completion */
        u8 ICC;
        /** 0x0F, Control register */
        u8 Control;

        /** 0x10 ~ 0x13 Reserved */
        u8 _1[4];
    } __attribute__((packed));

    struct Device_To_Host
    {
        /** 0x00, Type_Device_To_Host */
        Type FIS_Type : 8;

        /** 0x01:0, Port multiplier */
        u8 PMPort : 4;
        /** 0x01:4, Reserved */
        u8 _0 : 2;
        /** 0x01:6, Interrupt bit */
        u8 I : 1;
        /** 0x01:7, Reserved */
        u8 _1 : 1;

        /** 0x02, Status register */
        u8 Status;
        /** 0x03, Error register */
        u8 Error;

        /** 0x04, LBA low register, 7:0 */
        u8 LBA0;
        /** 0x05, LBA mid register, 15:8 */
        u8 LBA1;
        /** 0x06, LBA high register, 23:16 */
        u8 LBA2;
        /** 0x07, Device register */
        u8 Device;

        /** 0x08, LBA register, 31:24 */
        u8 LBA3;
        /** 0x09, LBA register, 39:32 */
        u8 LBA4;
        /** 0x0A, LBA register, 47:40 */
        u8 LBA5;
        /** 0x0B, Reserved */
        u8 _2;

        /** 0x0C, Count register, 7:0 */
        u8 CountLo;
        /** 0x0D, Count register, 15:8 */
        u8 CountHi;
        /** 0x0E ~ 0x0F, Reserved */
        u8 _3[2];

        /** 0x10 ~ 0x13, Reserved */
        u8 _4[4];
    } __attribute__((packed));

    struct Data
    {
        /** 0x00, Type_Data */
        Type FIS_Type : 8;

        /** 0x01:0, Port multiplier */
        u8 PMPort : 4;
        /** 0x01:4, Reserved */
        u8 _0 : 4;

        /** 0x02 ~ 0x03, Reserved */
        u8 _1[2];

        /** 0x04 ~ ..., Payload */
        u32 data[0];
    } __attribute__((packed));

    struct PIO_Setup
    {
        /** 0x00, Type_PIO_Setup */
        Type FIS_Type : 8;

        /** 0x01:0, Port multiplier */
        u8 PMPort : 4;
        /** 0x01:4, Reserved */
        u8 _0 : 1;
        /** 0x01:5, Data transfer direction, 1 - device to host */
        u8 D : 1;
        /** 0x01:6, Interrupt bit */
        u8 I : 1;
        /** 0x01:7, Reserved */
        u8 _1 : 1;

        /** 0x02, Status register */
        u8 Status;
        /** 0x03, Error register */
        u8 Error;

        /** 0x04, LBA low register, 7:0 */
        u8 LBA0;
        /** 0x05, LBA mid register, 15:8 */
        u8 LBA1;
        /** 0x06, LBA high register, 23:16 */
        u8 LBA2;
        /** 0x07, Device register */
        u8 Device;

        /** 0x08, LBA register, 31:24 */
        u8 LBA3;
        /** 0x09, LBA register, 39:32 */
        u8 LBA4;
        /** 0x0A, LBA register, 47:40 */
        u8 LBA5;
        /** 0x0B, Reserved */
        u8 _2;

        /** 0x0C, Count register, 7:0 */
        u8 CountLo;
        /** 0x0D, Count register, 15:8 */
        u8 CountHi;
        /** 0x0E, Reserved */
        u8 _3;
        /** 0x0F, New value of status register */
        u8 E_Status;

        /** 0x10, Transfer count */
        u16 TC;
        /** 0x12 ~ 0x13, Reserved */
        u8 _4[2];
    } __attribute__((packed));

    struct DMA_Setup
    {
        /** 0x00, Type_DMA_Setup */
        Type FIS_Type : 8;

        /** 0x01:0, Port multiplier */
        u8 PMPort : 4;
        /** 0x01:4, Reserved */
        u8 _0 : 1;
        /** 0x01:5, Data transfer direction, 1 - device to host */
        u8 D : 1;
        /** 0x01:6, Interrupt bit */
        u8 I : 1;
        /** 0x01:7, Auto-active. Specifies if DMA Activate FIS is needed */
        u8 A : 1;

        /** 0x02 ~ 0x03, Reserved */
        u8 _1[2];

        /** 0x04, DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and
         * not in Spec. Trying AHCI spec might work */
        u64 DMABufferID;

        /** 0x0C ~ 0x0F, Reserved */
        u8 _2[4];

        /** 0x10, Byte offset into buffer. First 2 bits must be 0 */
        u32 DMABufferOffset;

        /** 0x14, Number of bytes to transfer. Bit 0 must be 0 */
        u32 TransferCount;

        /** 0x18 ~ 0x1B, Reserved */
        u8 _3[4];
    } __attribute__((packed));

    struct Device_Bits
    {
        /** 0x00, Type_Device_Bits */
        Type FIS_Type : 8;

        // TODO
    } __attribute__((packed));
}
