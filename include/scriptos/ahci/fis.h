#pragma once

#include <scriptos/types.h>

namespace ahci::fis
{
    /**
     * Following code defines different kinds of FIS specified in Serial ATA
     * Revision 3.0.
     */
    enum TypeE
    {
        /** Register FIS - host to device */
        Type_HostToDevice = 0x27,
        /** Register FIS - device to host */
        Type_DeviceToHost = 0x34,
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
        Type_DeviceBits = 0xA1,
    };

    /**
     * A host to device register FIS is used by the host to send command or
     * control to a device. As illustrated in the following data structure, it
     * contains the IDE registers such as command, LBA, device, feature, count
     * and control. An ATA command is constructed in this structure and issued
     * to the device. All reserved fields in an FIS should be cleared to zero.
     */
    struct HostToDeviceT
    {
        /** FIS_TYPE_REG_H2D */
        TypeE Type : 8;

        /** Port Multiplier Port */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 Reserved0 : 3;
        /** 1: Command, 0: Control */
        u8 IsCommand : 1;

        /** Command register */
        u8 Command;
        /** Feature register, 7:0 */
        u8 FeatureLo;

        /** LBA low register, 7:0 */
        u8 LBA0;
        /** LBA mid register, 15:8 */
        u8 LBA1;
        /** LBA high register, 23:16 */
        u8 LBA2;
        /** Device register */
        u8 Device;

        /** LBA register, 31:24 */
        u8 LBA3;
        /** LBA register, 39:32 */
        u8 LBA4;
        /** LBA register, 47:40 */
        u8 LBA5;
        /** Feature register, 15:8 */
        u8 FeatureHi;

        /** Count register */
        u16 Count;
        /** Isochronous command completion */
        u8 ICC;
        /** Control register */
        u8 Control;

        /** Reserved */
        u8 Reserved1[4];
    } __attribute__((packed));

    /**
     * A device to host register FIS is used by the device to notify the host
     * that some ATA register has changed. It contains the updated task files
     * such as status, error and other registers.
     */
    struct DeviceToHostT
    {
        /** FIS_TYPE_REG_D2H */
        TypeE Type : 8;

        /** Port Multiplier Port */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 Reserved0 : 2;
        /** Interrupt bit */
        u8 Interrupt : 1;
        /** Reserved */
        u8 Reserved1 : 1;

        /** Status register */
        u8 Status;
        /** Error register */
        u8 Error;

        /** LBA low register, 7:0 */
        u8 LBA0;
        /** LBA mid register, 15:8 */
        u8 LBA1;
        /** LBA high register, 23:16 */
        u8 LBA2;
        /** Device register */
        u8 Device;

        /** LBA register, 31:24 */
        u8 LBA3;
        /** LBA register, 39:32 */
        u8 LBA4;
        /** LBA register, 47:40 */
        u8 LBA5;
        /** Reserved */
        u8 Reserved2;

        /** Count register */
        u16 Count;
        /** Reserved */
        u8 Reserved3[2];

        /** Reserved */
        u8 Reserved4[4];
    } __attribute__((packed));

    /**
     * This FIS is used by the host or device to send data payload. The data
     * size can be varied.
     */
    struct DataT
    {
        /** FIS_TYPE_DATA */
        TypeE Type : 8;

        /** Port Multiplier Port */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 Reserved0 : 4;

        /** Reserved */
        u8 Reserved1[2];

        /** Payload */
        u32 Payload[1];
    } __attribute__((packed));

    /**
     * This FIS is used by the device to tell the host that it’s about to send
     * or ready to receive a PIO data payload.
     */
    struct PIO_SetupT
    {
        /** FIS_TYPE_PIO_SETUP */
        TypeE Type : 8;

        /** Port Multiplier Port */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 Reserved0 : 1;
        /** Data transfer direction, 1 - device to host */
        u8 DataTransferDirection : 1;
        /** Interrupt bit */
        u8 Interrupt : 1;
        /** Reserved */
        u8 Reserved1 : 1;

        /** Status register */
        u8 Status;
        /** Error register */
        u8 Error;

        /** LBA low register, 7:0 */
        u8 LBA0;
        /** LBA mid register, 15:8 */
        u8 LBA1;
        /** LBA high register, 23:16 */
        u8 LBA2;
        /** Device register */
        u8 Device;

        /** LBA register, 31:24 */
        u8 LBA3;
        /** LBA register, 39:32 */
        u8 LBA4;
        /** LBA register, 47:40 */
        u8 LBA5;
        /** Reserved */
        u8 Reserved2;

        /** Count register */
        u16 Count;
        /** Reserved */
        u8 Reserved3;
        /** New value of status register */
        u8 ExtendedStatus;

        /** Transfer count */
        u16 TransferCount;
        /** Reserved */
        u8 Reserved4[2];
    } __attribute__((packed));

    struct DMA_SetupT
    {
        /** FIS_TYPE_DMA_SETUP */
        TypeE Type : 8;

        /** Port Multiplier Port */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 Reserved0 : 1;
        /** Data transfer direction, 1 - device to host */
        u8 DataTransferDirection : 1;
        /** Interrupt bit */
        u8 Interrupt : 1;
        /** Auto-activate. Specifies if DMA Activate FIS is needed */
        u8 AutoActive : 1;

        /** Reserved */
        u8 Reserved1[2];

        /**
         * DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
         * SATA Spec says host specific and not in Spec. Trying AHCI spec might
         * work.
         */
        u64 DMABufferID;

        /** Reserved */
        u8 Reserved2[4];

        /** Byte offset into buffer. First 2 bits must be 0 */
        u32 DMABufferOffset;

        /** Number of bytes to transfer. Bit 0 must be 0 */
        u32 TransferCount;

        /** Reserved */
        u8 Reserved3[4];
    } __attribute__((packed));

    struct DeviceBitsT
    {
        /** FIS_TYPE_DEV_BITS */
        TypeE Type : 8;

        /** Port multiplier */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 Reserved0 : 2;
        /** Interrupt */
        u8 Interrupt : 1;
        /** Reserved */
        u8 Reserved1 : 1;

        /** Device status register */
        u8 Status;
        /** Error register */
        u8 Error;

        /** Reserved or command-specific */
        u8 Vendor[4];
    } __attribute__((packed));
}
