#pragma once

#include <scriptos/types.h>

namespace ahci
{
    /**
     * Following code defines different kinds of FIS specified in Serial ATA
     * Revision 3.0.
     */
    enum FIS_TYPE
    {
        /** Register FIS - host to device */
        FIS_TYPE_REG_H2D = 0x27,
        /** Register FIS - device to host */
        FIS_TYPE_REG_D2H = 0x34,
        /** DMA activate FIS - device to host */
        FIS_TYPE_DMA_ACT = 0x39,
        /** DMA setup FIS - bidirectional */
        FIS_TYPE_DMA_SETUP = 0x41,
        /** Data FIS - bidirectional */
        FIS_TYPE_DATA = 0x46,
        /** BIST activate FIS - bidirectional */
        FIS_TYPE_BIST = 0x58,
        /** PIO setup FIS - device to host */
        FIS_TYPE_PIO_SETUP = 0x5F,
        /** Set device bits FIS - device to host */
        FIS_TYPE_DEV_BITS = 0xA1,
    };

    enum HBA_ISS
    {
        HBA_ISS_GEN_1 = 0b0001,
        HBA_ISS_GEN_2 = 0b0010,
        HBA_ISS_GEN_3 = 0b0011,
    };

    enum HBA_VS_MNR
    {
        HBA_VS_MNR_0_95 = 0x0905,
        HBA_VS_MNR_1_00 = 0x0000,
        HBA_VS_MNR_1_10 = 0x0100,
        HBA_VS_MNR_1_20 = 0x0200,
        HBA_VS_MNR_1_30 = 0x0300,
        HBA_VS_MNR_1_31 = 0x0301,
    };

    enum HBA_VS_MJR
    {
        HBA_VS_MJR_0_95 = 0x0000,
        HBA_VS_MJR_1_00 = 0x0001,
        HBA_VS_MJR_1_10 = 0x0001,
        HBA_VS_MJR_1_20 = 0x0001,
        HBA_VS_MJR_1_30 = 0x0001,
        HBA_VS_MJR_1_31 = 0x0001,
    };

    enum HBA_ICC
    {
        HBA_ICC_IDLE = 0x0,
        HBA_ICC_ACTIVE = 0x1,
        HBA_ICC_PARTIAL = 0x2,
        HBA_ICC_SLUMBER = 0x6,
        HBA_ICC_DEV_SLEEP = 0x8,
    };

    /**
     * A host to device register FIS is used by the host to send command or
     * control to a device. As illustrated in the following data structure, it
     * contains the IDE registers such as command, LBA, device, feature, count
     * and control. An ATA command is constructed in this structure and issued
     * to the device. All reserved fields in an FIS should be cleared to zero.
     */
    struct FIS_REG_H2D
    {
        /** FIS_TYPE_REG_H2D */
        FIS_TYPE FIS_Type : 8;

        /** Port Multiplier Port */
        u8 PMP : 4;
        /** Reserved */
        u8 RSV0 : 3;
        /** 1: Command, 0: Control */
        u8 CC : 1;

        /** Command register */
        u8 CMD;
        /** Feature register, 7:0 */
        u8 FL;

        /** LBA low register, 7:0 */
        u8 LBA0;
        /** LBA mid register, 15:8 */
        u8 LBA1;
        /** LBA high register, 23:16 */
        u8 LBA2;
        /** Device register */
        u8 DEV;

        /** LBA register, 31:24 */
        u8 LBA3;
        /** LBA register, 39:32 */
        u8 LBA4;
        /** LBA register, 47:40 */
        u8 LBA5;
        /** Feature register, 15:8 */
        u8 FH;

        /** Count register */
        u16 C;
        /** Isochronous command completion */
        u8 ICC;
        /** Control register */
        u8 CTL;

        /** Reserved */
        u8 RSV1[4];
    } __attribute__((packed));

    /**
     * A device to host register FIS is used by the device to notify the host
     * that some ATA register has changed. It contains the updated task files
     * such as status, error and other registers.
     */
    struct FIS_REG_D2H
    {
        /** FIS_TYPE_REG_D2H */
        FIS_TYPE FIS_Type : 8;

        /** Port Multiplier Port */
        u8 PMP : 4;
        /** Reserved */
        u8 RSV0 : 2;
        /** Interrupt bit */
        u8 I : 1;
        /** Reserved */
        u8 RSV1 : 1;

        /** Status register */
        u8 STS;
        /** Error register */
        u8 ERR;

        /** LBA low register, 7:0 */
        u8 LBA0;
        /** LBA mid register, 15:8 */
        u8 LBA1;
        /** LBA high register, 23:16 */
        u8 LBA2;
        /** Device register */
        u8 DEV;

        /** LBA register, 31:24 */
        u8 LBA3;
        /** LBA register, 39:32 */
        u8 LBA4;
        /** LBA register, 47:40 */
        u8 LBA5;
        /** Reserved */
        u8 RSV2;

        /** Count register, 15:0 */
        u16 C;
        /** Reserved */
        u8 RSV3[2];

        /** Reserved */
        u8 RSV4[4];
    } __attribute__((packed));

    /**
     * This FIS is used by the host or device to send data payload. The data
     * size can be varied.
     */
    struct FIS_DATA
    {
        /** FIS_TYPE_DATA */
        FIS_TYPE FIS_Type : 8;

        /** Port Multiplier Port */
        u8 PMP : 4;
        /** Reserved */
        u8 RSV0 : 4;

        /** Reserved */
        u8 RSV1[2];

        /** Payload */
        u32 DATA[0];
    } __attribute__((packed));

    /**
     * This FIS is used by the device to tell the host that it’s about to send
     * or ready to receive a PIO data payload.
     */
    struct FIS_PIO_Setup
    {
        /** FIS_TYPE_PIO_SETUP */
        FIS_TYPE FIS_Type : 8;

        /** Port Multiplier Port */
        u8 PMP : 4;
        /** Reserved */
        u8 RSV0 : 1;
        /** Data transfer direction, 1 - device to host */
        u8 DTD : 1;
        /** Interrupt bit */
        u8 I    : 1;
        u8 RSV1 : 1;

        /** Status register */
        u8 STS;
        /** Error register */
        u8 ERR;

        /** LBA low register, 7:0 */
        u8 LBA0;
        /** LBA mid register, 15:8 */
        u8 LBA1;
        /** LBA high register, 23:16 */
        u8 LBA2;
        /** Device register */
        u8 DEV;

        /** LBA register, 31:24 */
        u8 LBA3;
        /** LBA register, 39:32 */
        u8 LBA4;
        /** LBA register, 47:40 */
        u8 LBA5;
        /** Reserved */
        u8 RSV2;

        /** Count register */
        u16 C;
        /** Reserved */
        u8 RSV3;
        /** New value of status register */
        u8 ESTS;

        /** Transfer count */
        u16 TC;
        /** Reserved */
        u8 RSV4[2];
    } __attribute__((packed));

    struct FIS_DMA_Setup
    {
        /** FIS_TYPE_DMA_SETUP */
        FIS_TYPE FIS_Type : 8;

        /** Port Multiplier Port */
        u8 PMP : 4;
        /** Reserved */
        u8 RSV0 : 1;
        /** Data transfer direction, 1 - device to host */
        u8 DTD : 1;
        /** Interrupt bit */
        u8 I : 1;
        /** Auto-activate. Specifies if DMA Activate FIS is needed */
        u8 A : 1;

        /** Reserved */
        u8 RSV1[2];

        /**
         * DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
         * SATA Spec says host specific and not in Spec. Trying AHCI spec might
         * work.
         */
        u64 DMBBID;

        /** Reserved */
        u8 RSV2[4];

        /** Byte offset into buffer. First 2 bits must be 0 */
        u32 DMABOF;

        /** Number of bytes to transfer. Bit 0 must be 0 */
        u32 TC;

        /** Reserved */
        u8 RSV3[4];
    } __attribute__((packed));

    /**
     * This register indicates basic capabilities of the HBA to driver
     * software.
     */
    struct HBA_CAP
    {
        /** Number of Ports */
        const u8 NP : 5;
        /** Supports External SATA */
        const u8 SXS : 1;
        /** Enclosure Management Support */
        const u8 EMS : 1;
        /** Command Completion Coalescing Supported */
        const u8 CCCS : 1;
        /** Number of Command Slots */
        const u8 NCS : 5;
        /** Partial State Capable */
        const u8 PSC : 1;
        /** Slumber State Capable */
        const u8 SSC : 1;
        /** PIO Multiple DRQ Block */
        const u8 PMD : 1;
        /** FIS-based Switching Support */
        const u8 FBSS : 1;
        /** Supports Port Multiplier */
        const u8 SPM : 1;
        /** Supports AHCI Mode only */
        const u8 SAM : 1;
        /** Reserved */
        const u8 RSV : 1;
        /** Interface Speed Support */
        const HBA_ISS ISS : 4;
        /** Supports Command List Override */
        const u8 SCLO : 1;
        /** Supports Activity LED */
        const u8 SAL : 1;
        /** Supports Aggressive Link Power Management */
        const u8 SALP : 1;
        /** Supports Staggered Spin-up */
        const u8 SSS : 1;
        /** Supports Mechanical Presence Switch */
        const u8 SMPS : 1;
        /** Supports SNotification Register */
        const u8 SSNTF : 1;
        /** Supports Native Command Queuing */
        const u8 SNCQ : 1;
        /** Supports 64-bit Addressing */
        const u8 S64A : 1;
    } __attribute__((packed));

    /**
     * This register controls various global actions of the HBA.
     */
    struct HBA_GHC
    {
        /** HBA Reset */
        u8 HR : 1;
        /** Interrupt Enable */
        u8 IE : 1;
        /** MSI Revert to Single Message */
        const u8 MRSM : 1;
        /** Reserved */
        const u32 RSV : 28;
        /** AHCI Enable */
        u8 AE : 1;
    } __attribute__((packed));

    /**
     * This register indicates the major and minor version of the AHCI
     * specification that the HBA implementation supports. The upper two bytes
     * represent the major version number, and the lower two bytes represent the
     * minor version number. Example: Version 3.12 would be represented as
     * 00030102h. Three versions of the specification are valid:
     * 0.95, 1.0, 1.1, 1.2, 1.3, and 1.3.1
     */
    struct HBA_VS
    {
        /** Minor Version Number */
        const HBA_VS_MNR MNR : 16;
        /** Major Version Number */
        const HBA_VS_MJR MJR : 16;
    } __attribute__((packed));

    /**
     * The command completion coalescing control register is used to configure
     * the command completion coalescing feature for the entire HBA.
     */
    struct HBA_CCC_CTL
    {
        /** Enable */
        u8 EN : 1;
        /** Reserved */
        const u8 RSV : 2;
        /** Interrupt */
        const u8 INT : 5;
        /** Command Completions */
        u8 CC;
        /** Timeout Value */
        u16 TV;
    } __attribute__((packed));

    /**
     * The enclosure management location register identifies the location and
     * size of the enclosure management message buffer.
     */
    struct HBA_EM_LOC
    {
        /** Buffer Size */
        const u16 SZ;
        /** Offset */
        const u16 OFST;
    } __attribute__((packed));

    /**
     * This register is used to control and obtain status for the enclosure
     * management interface. The register includes information on the attributes
     * of the implementation, enclosure management messages supported, the
     * status of the interface, whether any messages are pending, and is used to
     * initiate sending messages.
     */
    struct HBA_EM_CTL
    {
        /** Message Received */
        u8 STS_MR : 1;
        /** Reserved */
        const u8 RSV0 : 7;
        /** Transmit Message */
        u8 CTL_TM : 1;
        /** Reset */
        u8 CTL_RST : 1;
        /** Reserved */
        const u8 RSV1 : 6;
        /** LED Message Types */
        const u8 SUPP_LED : 1;
        /** SAF-TE Enclosure Management Message */
        const u8 SUPP_SAFTE : 1;
        /** SES-2 Enclosure Management Message */
        const u8 SUPP_SES2 : 1;
        /** SGPIO Enclosure Management Message */
        const u8 SUPP_SGPIO : 1;
        /** Reserved */
        const u8 RSV2 : 4;
        /** Single Message Buffer */
        const u8 ATTR_SMB : 1;
        /** Transmit Only */
        const u8 ATTR_XMT : 1;
        /** Activity LED Hardware Driven */
        const u8 ATTR_ALHD : 1;
        /** Port Multiplier Support */
        const u8 ATTR_PM : 1;
        /** Reserved */
        const u8 RSV3 : 4;
    } __attribute__((packed));

    struct HBA_CAP2
    {
        /** BIOS/OS Handoff */
        const u8 BOH : 1;
        /** NVMHCI Present */
        const u8 NVMP : 1;
        /** Automatic Partial to Slumber Transition */
        const u8 APST : 1;
        /** Supports Device Sleep */
        const u8 SDS : 1;
        /** Supports Aggressive Device Sleep Management */
        const u8 SADM : 1;
        /** DevSleep Entrance from Slumber Only */
        const u8 DESO : 1;
        /** Reserved */
        const u32 RSV : 26;
    } __attribute__((packed));

    /**
     * This register controls various global actions of the HBA. This register
     * is not affected by an HBA reset.
     */
    struct HBA_BOHC
    {
        /** BIOS Owned Semaphore */
        u8 BOS : 1;
        /** OS Owned Semaphore */
        u8 OOS : 1;
        /** SMI on OS Ownership Change Enable */
        u8 SOOE : 1;
        /** OS Ownership Change */
        u8 OOC : 1;
        /** BIOS Busy */
        u8 BB : 1;
        /** Reserved */
        u32 RSV : 27;
    } __attribute__((packed));

    union HBA_PxIS
    {
        u32 Value;

        struct
        {
            /** Device to Host Register FIS Interrupt */
            u8 DHRS : 1;
            /** PIO Setup FIS Interrupt */
            u8 PSS : 1;
            /** DMA Setup FIS Interrupt */
            u8 DSS : 1;
            /** Set Device Bits Interrupt */
            u8 SDBS : 1;
            /** Unknown FIS Interrupt */
            const u8 UFS : 1;
            /** Descriptor Processed */
            u8 DPS : 1;
            /** Port Connect Change Status */
            const u8 PCS : 1;
            /** Device Mechanical Presence Status */
            u8 DMPS : 1;
            /** Reserved */
            const u16 RSV0 : 14;
            /** PhyRdy Change Status */
            const u8 PRCS : 1;
            /** Incorrect Port Multiplier Status */
            u8 IPMS : 1;
            /** Overflow Status */
            u8 OFS : 1;
            /** Reserved */
            const u8 RSV1 : 1;
            /** Interface Non-fatal Error Status */
            u8 INFS : 1;
            /** Interface Fatal Error Status */
            u8 IFS : 1;
            /** Host Bus Data Error Status */
            u8 HBDS : 1;
            /** Host Bus Fatal Error Status */
            u8 HBFS : 1;
            /** Task File Error Status */
            u8 TFES : 1;
            /** Cold Port Detect Status */
            u8 CPDS : 1;
        } __attribute__((packed));
    };

    /**
     * This register enables and disables the reporting of the corresponding
     * interrupt to system software. When a bit is set (‘1’) and the
     * corresponding interrupt condition is active, then an interrupt is
     * generated. Interrupt sources that are disabled (‘0’) are still reflected
     * in the status registers. This register is symmetrical with the PxIS
     * register.
     */
    struct HBA_PxIE
    {
        /** Device to Host Register FIS Interrupt Enable */
        u8 DHRE : 1;
        /** PIO Setup FIS Interrupt Enable */
        u8 PSE : 1;
        /** DMA Setup FIS Interrupt Enable */
        u8 DSE : 1;
        /** Set Device Bits FIS Interrupt Enable */
        u8 SDBE : 1;
        /** Unknown FIS Interrupt Enable */
        u8 UFE : 1;
        /** Descriptor Processed Interrupt Enable */
        u8 DPE : 1;
        /** Port Change Interrupt Enable */
        u8 PCE : 1;
        /** Device Mechanical Presence Enable */
        u8 DMPE : 1;
        /** Reserved */
        const u16 RSV0 : 14;
        /** PhyRdy Change Interrupt Enable */
        u8 PRCE : 1;
        /** Incorrect Port Multiplier Enable */
        u8 IPME : 1;
        /** Overflow Enable */
        u8 OFE : 1;
        /** Reserved */
        const u8 RSV1 : 1;
        /** Interface Non-fatal Error Enable */
        u8 INFE : 1;
        /** Interface Fatal Error Enable */
        u8 IFE : 1;
        /** Host Bus Data Error Enable */
        u8 HBDE : 1;
        /** Host Bus Fatal Error Enable */
        u8 HBFE : 1;
        /** Task File Error Enable */
        u8 TFEE : 1;
        /** Cold Presence Detect Enable */
        u8 CPDE : 1;
    } __attribute__((packed));

    struct HBA_PxCMD
    {
        /** Start */
        u8 ST : 1;
        /** Spin-Up Device */
        u8 SUD : 1;
        /** Power On Device */
        u8 POD : 1;
        /** Command List Override */
        u8 CLO : 1;
        /** FIS Receive Enable */
        u8 FRE : 1;
        /** Reserved */
        const u8 RSV : 3;
        /** Current Command Slot */
        const u8 CCS : 5;
        /** Mechanical Presence Switch State */
        const u8 MPSS : 1;
        /** FIS Receive Running */
        const u8 FR : 1;
        /** Command List Running */
        const u8 CR : 1;
        /** Cold Presence State */
        const u8 CPS : 1;
        /** Port Multiplier Attached */
        u8 PMA : 1;
        /** Hot Plug Capable Port */
        const u8 HPCP : 1;
        /** Mechanical Presence Switch Attached to Port */
        const u8 MPSP : 1;
        /** Cold Presence Detection */
        const u8 CPD : 1;
        /** External SATA Port */
        const u8 ESP : 1;
        /** FIS-based Switching Capable Port */
        const u8 FBSCP : 1;
        /** Automatic Partial to Slumber Transitions Enable */
        u8 APSTE : 1;
        /** Device is ATAPI */
        u8 ATAPI : 1;
        /** Drive LED on ATAPI Enable */
        u8 DLAE : 1;
        /** Aggressive Link Power Management Enable */
        u8 ALPE : 1;
        /** Aggressive Slumber / Partial */
        u8 ASP : 1;
        /** Interface Communication Control */
        HBA_ICC ICC : 4;
    } __attribute__((packed));

    /**
     * This is a 32-bit register that copies specific fields of the task file
     * when FISes are received. The FISes that contain this information are:
     * - D2H Register FIS
     * - PIO Setup FIS
     * - Set Device Bits FIS (BSY and DRQ are not updated with this FIS)
     */
    struct HBA_PxTFD
    {
        /** Indicates an error during the transfer. */
        u8 STS_ERR : 1;
        /** Command specific */
        u8 STS_CS0 : 2;
        /** Indicates a data transfer is requested */
        u8 STS_DRQ : 1;
        /** Command specific */
        u8 STS_CS1 : 3;
        /** Indicates the interface is busy */
        u8 STS_BSY : 1;

        /** Error */
        const u8 ERR;
        /** Reserved */
        const u16 RSV;
    } __attribute__((packed));

    union HBA_PxSIG
    {
        u32 Value;

        struct
        {
            /** Sector Count */
            u8 SC;
            /** LBA Low */
            u8 LBAL;
            /** LBA Mid */
            u8 LBAM;
            /** LBA High */
            u8 LBAH;
        } __attribute__((packed));
    };

    /**
     * This 32-bit register conveys the current state of the interface and host.
     * The HBA updates it continuously and asynchronously. When the HBA
     * transmits a COMRESET to the device, this register is updated to its reset
     * values.
     */
    struct HBA_PxSSTS
    {
        /** Device Detection */
        const u8 DET : 4;
        /** Current Interface Speed */
        const u8 SPD : 4;
        /** Interface Power Management */
        const u8 IPM : 4;
        /** Reserved */
        const u32 RSV : 20;
    } __attribute__((packed));

    /**
     * This is a 32-bit read-write register by which software controls SATA
     * capabilities. Writes to this register result in an action being taken by
     * the host adapter or interface. Reads from the register return the last
     * value written to it.
     */
    struct HBA_PxSCTL
    {
        /** Device Detection Initialization */
        u8 DET : 4;
        /** Speed Allowed */
        u8 SPD : 4;
        /** Interface Power Management Transition Allowed */
        u8 IPM : 4;
        /** Select Power Management */
        const u8 SPM : 4;
        /** Port Multiplier Port */
        const u8 PMP : 4;
        /** Reserved */
        const u16 RSV : 12;
    } __attribute__((packed));

    struct HBA_PxSERR
    {
        /** Error */
        u16 ERR;
        /** Diagnostics */
        u16 DIAG;
    } __attribute__((packed));

    /**
     * This register is used to control and obtain status for Port Multiplier
     * FIS-based switching.
     */
    struct HBA_PxFBS
    {
        /** Enable */
        u8 EN : 1;
        /** Device Error Clear */
        u8 DEC : 1;
        /** Single Device Error */
        const u8 SDE : 1;
        /** Reserved */
        const u8 RSV0 : 5;
        /** Device to Issue */
        u8 DEV : 4;
        /** Active Device Optimization */
        const u8 ADO : 4;
        /** Device with Error */
        const u8 DWE : 4;
        /** Reserved */
        const u16 RSV1 : 12;
    } __attribute__((packed));

    struct HBA_PxDEVSLP
    {
        /** Aggressive Device Sleep Enable */
        u8 ADSE : 1;
        /** Device Sleep Present */
        const u8 DSP : 1;
        /** Device Sleep Exit Timeout */
        u8 DETO : 8;
        /** Minimum Device Sleep Assertion Time */
        u8 MDAT : 5;
        /** Device Sleep Idle Timeout */
        u16 DITO : 10;
        /** DITO Multiplier */
        const u8 DM : 4;
        /** Reserved */
        const u8 RSV : 3;
    } __attribute__((packed));

    struct HBA_PORT
    {
        /** Command List Base Address (1K) */
        u64 CLB;
        /** FIS Base Address (1K) */
        u64 FB;
        /** Interrupt Status */
        HBA_PxIS IS;
        /** Interrupt Enable */
        HBA_PxIE IE;
        /** Command and Status */
        HBA_PxCMD CMD;
        /** Reserved */
        u8 RSV0[4];
        /** Task File Data */
        HBA_PxTFD TFD;
        /** Signature */
        HBA_PxSIG SIG;
        /** SATA Status */
        HBA_PxSSTS SSTS;
        /** SATA Control */
        HBA_PxSCTL SCTL;
        /** SATA Error */
        HBA_PxSERR SERR;
        /** SATA Active */
        u32 SACT;
        /** Command Issue */
        u32 CI;
        /** SATA Notification */
        u32 SNTF;
        /** FIS-based Switch Control */
        HBA_PxFBS FBS;
        /** Device Sleep */
        HBA_PxDEVSLP DEVSLP;
        /** Reserved */
        u8 RSV1[0x70 - 0x48];
        /** Vendor Specific */
        u8 VS[0x80 - 0x70];
    } __attribute__((packed));

    struct HBA_MEM
    {
        struct Iterator
        {
            bool operator==(const Iterator& iterator) const;
            HBA_PORT* operator*() const;
            Iterator& operator++();

            u32 PI;
            HBA_PORT* PCR;
            u8 Index;
        };

        Iterator begin();
        Iterator end();

        /** Host Capabilities */
        HBA_CAP CAP;
        /** Global Host Control */
        HBA_GHC GHC;
        /** Interrupt Status */
        u32 IS;
        /** Port Implemented */
        u32 PI;
        /** Version */
        HBA_VS VS;
        /** Command Completion Coalescing Control */
        HBA_CCC_CTL CCC_CTL;
        /** Command Completion Coalescing Ports */
        u32 CCC_PORTS;
        /** Enclosure Management Location */
        HBA_EM_LOC EM_LOC;
        /** Enclosure Management Control */
        HBA_EM_CTL EM_CTL;
        /** Host Capabilities Extended */
        HBA_CAP2 CAP2;
        /** BIOS/OS Handoff Control / Status */
        HBA_BOHC BOHC;
        /** Reserved */
        u8 RSV[0xA0 - 0x2C];
        /** Vendor Specific Registers */
        u8 VND[0x100 - 0xA0];
        /** Port Control Registers */
        HBA_PORT PCR[0];
    } __attribute__((packed));

    struct HBA_CMD_HEADER
    {
        /** Command FIS Length */
        u8 CFL : 5;
        /** ATAPI */
        u8 A : 1;
        /** Write */
        u8 W : 1;
        /** Prefetchable */
        u8 P : 1;
        /** Reset */
        u8 R : 1;
        /** BIST */
        u8 B : 1;
        /** Clear busy upon R_OK */
        u8 C : 1;
        /** Reserved */
        u8 RSV0 : 1;
        /** Port Multiplier Port */
        u8 PMP : 4;
        /** Physical Region Descriptor Table Length */
        u16 PRDTL;

        /** Physical Region Descriptor Byte Count */
        u32 PRDBC;

        /** Command Table Descriptor Base Address */
        u64 CTBA;

        /** Reserved */
        u8 RSV1[16];
    } __attribute__((packed));

    /**
     * This table contains the scatter / gather list for the data transfer. It
     * contains a list of 0 (no data to transfer) to up to 65,535 entries. A
     * breakdown of each field in a PRD table is shown below. Item 0 refers to
     * the first entry in the PRD table. Item "CH[PRDTL] – 1" refers to the last
     * entry in the table, where the length field comes from the PRDTL field in
     * the command list entry for this command slot.
     */
    struct HBA_PRDT
    {
        /** Data Base Address */
        u64 DBA;
        /** Reserved */
        u8 RSV0[4];

        /** Data Byte Count */
        u32 DBC : 22;
        /** Reserved */
        u32 RSV1 : 9;
        /** Interrupt on Completion */
        u32 I : 1;
    } __attribute__((packed));

    struct HBA_CMD_TBL
    {
        /** Command FIS */
        u8 CFIS[64];

        /** ATAPI command, 12 or 16 bytes */
        u8 ATAPI_Command[16];

        /** Reserved */
        u8 RSV[48];

        /** Physical region descriptor table entries, 0 ~ 65535 */
        HBA_PRDT PRDT[0];
    } __attribute__((packed));

    struct FIS_DEV_BITS
    {
        /** FIS_TYPE_DEV_BITS */
        u8 FIS_Type;

        /** Port multiplier */
        u8 PortMultiplierPort : 4;
        /** Reserved */
        u8 RSV0 : 2;
        /** Interrupt */
        u8 Interrupt : 1;
        /** Reserved */
        u8 RSV1 : 1;

        /** Device status register */
        u8 Status;
        /** Error register */
        u8 Error;

        /** Reserved or command-specific */
        u8 Vendor[4];
    } __attribute__((packed));

    struct HBA_FIS
    {
        /** DMA Setup FIS */
        FIS_DMA_Setup DMA_Setup_FIS;
        u8 PAD0[0x04];

        /** PIO Setup FIS */
        FIS_PIO_Setup PIO_Setup_FIS;
        u8 PAD1[0x0C];

        /** Register - device to host FIS */
        FIS_REG_D2H Register_FIS;
        u8 PAD2[0x04];

        /** Set device bit FIS */
        FIS_DEV_BITS SetDeviceBit_FIS;

        u8 UFIS[0xA0 - 0x60];
        u8 RSV[0x100 - 0xA0];
    } __attribute__((packed));

    void BeginCommand(HBA_PORT* port);
    void EndCommand(HBA_PORT* port);

    void RebasePort(
        HBA_PORT* port,
        uptr base_address,
        unsigned index);

    int FindCommandSlot(HBA_PORT* port);

    bool Read(
        HBA_PORT* port,
        uptr offset,
        u16 count,
        u16* buffer);
}
