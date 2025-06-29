#pragma once

#include <scriptos/ahci/fis.h>
#include <scriptos/stl.h>
#include <scriptos/types.h>

namespace ahci::hba
{
    enum ISS_E
    {
        HBA_ISS_GEN_1 = 0b0001,
        HBA_ISS_GEN_2 = 0b0010,
        HBA_ISS_GEN_3 = 0b0011,
    };

    enum VersionE
    {
        HBA_VS_0_95 = 0x00000905,
        HBA_VS_1_00 = 0x00010000,
        HBA_VS_1_10 = 0x00010100,
        HBA_VS_1_20 = 0x00010200,
        HBA_VS_1_30 = 0x00010300,
        HBA_VS_1_31 = 0x00010301,
    };

    enum ICC_E
    {
        HBA_ICC_IDLE = 0x0,
        HBA_ICC_ACTIVE = 0x1,
        HBA_ICC_PARTIAL = 0x2,
        HBA_ICC_SLUMBER = 0x6,
        HBA_ICC_DEV_SLEEP = 0x8,
    };

    enum DeviceDetectionE
    {
        HBA_PORT_DET_PRESENT = 0b0011,
    };

    enum InterfacePowerManagementE
    {
        HBA_PORT_IPM_ACTIVE = 0b0001,
    };

    enum SignatureE
    {
        HBA_PORT_SIG_ATA = 0x00000101,
        HBA_PORT_SIG_ATAPI = 0xEB140101,
        HBA_PORT_SIG_SEMB = 0xC33C0101,
        HBA_PORT_SIG_PM = 0x96690101,
    };

    struct CAP_T
    {
        /**
         * Number of Ports
         *
         * 0’s based value indicating the maximum number of ports supported by
         * the HBA silicon. A maximum of 32 ports can be supported. A value of
         * ‘0h’, indicating one port, is the minimum requirement. Note that the
         * number of ports indicated in this field may be more than the number
         * of ports indicated in the PI register.
         */
        const u8 NP : 5;
        /**
         * Supports External SATA
         *
         * When set to ‘1’, indicates that the HBA has one or more Serial ATA
         * ports that has a signal only connector that is externally accessible
         * (e.g. eSATA connector). If this bit is set to ‘1’, software may refer
         * to the PxCMD.ESP bit to determine whether a specific port has its
         * signal connector externally accessible as a signal only connector
         * (i.e. power is not part of that connector). When the bit is cleared
         * to ‘0’, indicates that the HBA has no Serial ATA ports that have a
         * signal only connector externally accessible.
         */
        const u8 SXS : 1;
        /**
         * Enclosure Management Support
         *
         * When set to ‘1’, indicates that the HBA supports enclosure management
         * as defined in section 12. When enclosure management is supported, the
         * HBA has implemented the EM_LOC and EM_CTL global HBA registers. When
         * cleared to ‘0’, indicates that the HBA does not support enclosure
         * management and the EM_LOC and EM_CTL global HBA registers are not
         * implemented.
         */
        const u8 EMS : 1;
        /**
         * Command Completion Coalescing Supported
         *
         * When set to ‘1’, indicates that the HBA supports command completion
         * coalescing as defined in section 11. When command completion
         * coalescing is supported, the HBA has implemented the CCC_CTL and the
         * CCC_PORTS global HBA registers. When cleared to ‘0’, indicates that
         * the HBA does not support command completion coalescing and the
         * CCC_CTL and CCC_PORTS global HBA registers are not implemented.
         */
        const u8 CCCS : 1;
        /**
         * Number of Command Slots
         *
         * 0’s based value indicating the number of command slots per port
         * supported by this HBA. A minimum of 1 and maximum of 32 slots per
         * port can be supported. The same number of command slots is available
         * on each implemented port.
         */
        const u8 NCS : 5;
        /**
         * Partial State Capable
         *
         * Indicates whether the HBA can support transitions to the Partial
         * state. When cleared to ‘0’, software must not allow the HBA to
         * initiate transitions to the Partial state via agressive link power
         * management nor the PxCMD.ICC field in each port, and the PxSCTL.IPM
         * field in each port must be programmed to disallow device initiated
         * Partial requests. When set to ‘1’, HBA and device initiated Partial
         * requests can be supported.
         */
        const u8 PSC : 1;
        /**
         * Slumber State Capable
         *
         * Indicates whether the HBA can support transitions to the Slumber
         * state. When cleared to ‘0’, software must not allow the HBA to
         * initiate transitions to the Slumber state via agressive link power
         * management nor the PxCMD.ICC field in each port, and the PxSCTL.IPM
         * field in each port must be programmed to disallow device initiated
         * Slumber requests. When set to ‘1’, HBA and device initiated Slumber
         * requests can be supported.
         */
        const u8 SSC : 1;
        /**
         * PIO Multiple DRQ Block
         *
         * If set to ‘1’, the HBA supports multiple DRQ block data transfers for
         * the PIO command protocol. If cleared to ‘0’ the HBA only supports
         * single DRQ block data transfers for the PIO command protocol.
         * AHCI 1.2 HBAs shall have this bit set to ‘1’.
         */
        const u8 PMD : 1;
        /**
         * FIS-based Switching Support
         *
         * When set to ‘1’, indicates that the HBA supports Port Multiplier
         * FIS-based switching. When cleared to ‘0’, indicates that the HBA does
         * not support FIS-based switching. This bit shall only be set to ‘1’ if
         * the SPM bit is set to ‘1’.
         */
        const u8 FPSS : 1;
        /**
         * Supports Port Multiplier
         *
         * Indicates whether the HBA can support a Port Multiplier. When set, a
         * Port Multiplier using command-based switching is supported and
         * FIS-based switching may be supported. When cleared to ‘0’, a Port
         * Multiplier is not supported, and a Port Multiplier may not be
         * attached to this HBA.
         */
        const u8 SPM : 1;
        /**
         * Supports AHCI Mode only
         *
         * The SATA controller may optionally support AHCI access mechanisms
         * only. A value of '0' indicates that in addition to the native AHCI
         * mechanism (via ABAR), the SATA controller implements a legacy,
         * task-file based register interface such as SFF-8038i. A value of '1'
         * indicates that the SATA controller does not implement a legacy,
         * task-file based register interface.
         */
        const u8 SAM : 1;
        /** Reserved */
        const u8 RSV0 : 1;
        /**
         * Interface Speed Support
         *
         * Indicates the maximum speed the HBA can support on its ports. These
         * encodings match the system software programmable PxSCTL.DET.SPD
         * field. Values are:
         *
         * - 0000        : Reserved
         * - 0001        : Gen 1 (1.5 Gbps)
         * - 0010        : Gen 2 (3   Gbps)
         * - 0011        : Gen 3 (6   Gbps)
         * - 0100 - 1111 : Reserved
         */
        const ISS_E ISS : 4;
        /**
         * Supports Command List Override
         *
         * When set to ‘1’, the HBA supports the PxCMD.CLO bit and its
         * associated function. When cleared to ‘0’, the HBA is not capable of
         * clearing the BSY and DRQ bits in the Status register in order to
         * issue a software reset if these bits are still set from a previous
         * operation.
         */
        const u8 SCLO : 1;
        /**
         * Supports Activity LED
         *
         * When set to ‘1’, the HBA supports a single activity indication output
         * pin. This pin can be connected to an LED on the platform to indicate
         * device activity on any drive. When cleared to ‘0’, this function is
         * not supported. See section 10.11 for more information.
         */
        const u8 SAL : 1;
        /**
         * Supports Aggressive Link Power Management
         *
         * When set to ‘1’, the HBA can support auto-generating link requests to
         * the Partial or Slumber states when there are no commands to process.
         * When cleared to ‘0’, this function is not supported and software
         * shall treat the PxCMD.ALPE and PxCMD.ASP bits as reserved. Refer to
         * section 8.3.1.3.
         */
        const u8 SALP : 1;
        /**
         * Supports Staggered Spin-up
         *
         * When set to ‘1’, the HBA supports staggered spin-up on its ports, for
         * use in balancing power spikes. When cleared to ‘0’, this function is
         * not supported. This value is loaded by the BIOS prior to OS
         * initiallization.
         */
        const u8 SSS : 1;
        /**
         * Supports Mechanical Presence Switch
         *
         * When set to ‘1’, the HBA supports mechanical presence switches on its
         * ports for use in hot plug operations. When cleared to ‘0’, this
         * function is not supported. This value is loaded by the BIOS prior to
         * OS initialization.
         */
        const u8 SMPS : 1;
        /**
         * Supports SNotification Register
         *
         * When set to ‘1’, the HBA supports the PxSNTF (SNotification) register
         * and its associated functionality. When cleared to ‘0’, the HBA does
         * not support the PxSNTF (SNotification) register and its associated
         * functionality. Refer to section 10.11.1. Asynchronous notification
         * with a directly attached device is always supported.
         */
        const u8 SSNTF : 1;
        /**
         * Supports Native Command Queuing
         *
         * Indicates whether the HBA supports Serial ATA native command queuing.
         * If set to ‘1’, an HBA shall handle DMA Setup FISes natively, and
         * shall handle the auto-activate optimization through that FIS. If
         * cleared to ‘0’, native command queuing is not supported and software
         * should not issue any native command queuing commands.
         */
        const u8 SNCQ : 1;
        /**
         * Supports 64-bit Addressing
         *
         * Indicates whether the HBA can access 64-bit data structures. When set
         * to ‘1’, the HBA shall make the 32-bit upper bits of the port DMA
         * Descriptor, the PRD Base, and each PRD entry read/write. When cleared
         * to ‘0’, these are read-only and treated as ‘0’ by the HBA.
         */
        const u8 S64A : 1;
    } __attribute__((packed));

    struct GHC_T
    {
        /**
         * HBA Reset
         *
         * When set by SW, this bit causes an internal reset of the HBA. All
         * state machines that relate to data transfers and queuing shall return
         * to an idle condition, and all ports shall be re-initialized via
         * COMRESET (if staggered spin-up is not supported). If staggered
         * spin-up is supported, then it is the responsibility of software to
         * spin-up each port after the reset has completed.
         *
         * When the HBA has performed the reset action, it shall reset this bit
         * to ‘0’. A software write of ‘0’ shall have no effect. For a
         * description on which bits are reset when this bit is set, see
         * section 10.4.3.
         */
        u8 HR : 1;
        /**
         * Interrupt Enable
         *
         * This global bit enables interrupts from the HBA. When cleared (reset
         * default), all interrupt sources from all ports are disabled. When
         * set, interrupts are enabled.
         */
        u8 IE : 1;
        /**
         * MSI Revert to Single Message
         *
         * When set to ‘1’ by hardware, indicates that the HBA requested more
         * than one MSI vector but has reverted to using the first vector only.
         * When this bit is cleared to ‘0’, the HBA has not reverted to single
         * MSI mode (i.e. hardware is already in single MSI mode, software has
         * allocated the number of messages requested, or hardware is sharing
         * interrupt vectors if MC.MME < MC.MMC). The HBA may revert to single
         * MSI mode when the number of vectors allocated by the host is less
         * than the number requested. This bit shall only be set to ‘1’ when
         * the following conditions hold:
         *
         * - MC.MSIE = ‘1’ (MSI is enabled)
         * - MC.MMC > 0 (multiple messages requested)
         * - MC.MME > 0 (more than one message allocated)
         * - MC.MME != MC.MMC (messages allocated not equal to number requested)
         *
         * When this bit is set to ‘1’, single MSI mode operation is in use and software is responsible for clearing bits in the IS register to clear interrupts. This bit shall be cleared to ‘0’ by hardware when any of the four conditions stated is false. This bit is also cleared to ‘0’ when MC.MSIE = ‘1’ and MC.MME = 0h. In this case, the hardware has been programmed to use single MSI mode, and is not “reverting” to that mode.
         */
        const u8 MRSM : 1;
        /** Reserved */
        const u32 RSV0 : 28;
        /**
         * AHCI Enable
         *
         * When set, indicates that communication to the HBA shall be via AHCI
         * mechanisms. This can be used by an HBA that supports both legacy
         * mechanisms (such as SFF-8038i) and AHCI to know when the HBA is
         * running under an AHCI driver.
         *
         * When set, software shall only communicate with the HBA using AHCI.
         * When cleared, software shall only communicate with the HBA using
         * legacy mechanisms. When cleared FISes are not posted to memory and no
         * commands are sent via AHCI mechanisms.
         *
         * Software shall set this bit to ‘1’ before accessing other AHCI
         * registers. When software clears this bit to ‘0’ from a previous value
         * of ‘1’, it shall set no other bit in the GHC register as part of that
         * operation (i.e., clearing the AE bit requires software to write
         * 00000000h to the register).
         *
         * The implementation of this bit is dependent upon the value of the
         * CAP.SAM bit. If CAP.SAM is '0', then GHC.AE shall be read-write and
         * shall have a reset value of '0'. If CAP.SAM is '1', then AE shall be
         * read-only and shall have a reset value of '1'.
         */
        u8 AE : 1;
    } __attribute__((packed));

    struct CCC_CTL_T
    {
        /**
         * Enable
         *
         * When cleared to ‘0’, the command completion coalescing feature is
         * disabled and no CCC interrupts are generated. When set to ‘1’, the
         * command completion coalescing feature is enabled and CCC interrupts
         * may be generated based on timeout or command completion conditions.
         * Software shall only change the contents of the TV and CC fields when
         * EN is cleared to ‘0’. On transition of this bit from ‘0’ to ‘1’, any
         * updated values for the TV and CC fields shall take effect.
         */
        u8 EN : 1;
        /** Reserved */
        const u8 RSV0 : 2;
        /**
         * Interrupt
         *
         * Specifies the interrupt used by the CCC feature. This interrupt must
         * be marked as unused in the Ports Implemented (PI) register by the
         * corresponding bit being set to ‘0’. Thus, the CCC interrupt
         * corresponds to the interrupt for an unimplemented port on the
         * controller. When a CCC interrupt occurs, the IS.IPS[INT] bit shall be
         * asserted to ‘1’. This field also specifies the interrupt vector used
         * for MSI.
         */
        const u8 INT : 5;
        /**
         * Command Completions
         *
         * Specifies the number of command completions that are necessary to
         * cause a CCC interrupt. The HBA has an internal command completion
         * counter, hCccComplete. hCccComplete is incremented by one each time a
         * selected port has a command completion. When hCccComplete is equal to
         * the command completions value, a CCC interrupt is signaled. The
         * internal command completion counter is reset to ‘0’ on the assertion
         * of each CCC interrupt. A value of ‘0’ for this field shall disable
         * CCC interrupts being generated based on the number of commands
         * completed, i.e. CCC interrupts are only generated based on the timer
         * in this case.
         */
        u8 CommandCompletions;
        /**
         * Timeout Value
         *
         * The timeout value is specified in 1 millisecond intervals. The timer
         * accuracy shall be within 5%. hCccTimer is loaded with this timeout
         * value. hCccTimer is only decremented when commands are outstanding on
         * selected ports, as defined in section 11.2. The HBA will signal a CCC
         * interrupt when hCccTimer has decremented to ‘0’. hCccTimer is reset
         * to the timeout value on the assertion of each CCC interrupt. A
         * timeout value of ‘0’ is reserved.
         */
        u16 TV;
    } __attribute__((packed));

    struct EM_LOC_T
    {
        /**
         * Buffer Size
         *
         * Specifies the size of the transmit message buffer area in Dwords. If
         * both transmit and receive buffers are supported, then the transmit
         * buffer begins at ABAR[EM_LOC.OFST*4] and the receive buffer directly
         * follows it. If both transmit and receive buffers are supported, both
         * buffers are of the size indicated in the Buffer Size field. A value
         * of ‘0’ is invalid.
         */
        const u16 SZ;
        /**
         * Offset
         *
         * The offset of the message buffer in Dwords from the beginning of the
         * ABAR.
         */
        const u16 OFST;
    } __attribute__((packed));

    struct EM_CTL_T
    {
        /**
         * Message Received
         *
         * The HBA sets this bit to a ‘1’ when a message is completely received
         * into the message buffer. When software detects this bit is a ‘1’,
         * software should read the message and perform any actions necessary.
         * When software is finished reading the message in the buffer, software
         * writes a ‘1’ to this bit in order to clear it. A write of ‘0’ to this
         * bit by software shall have no effect.
         */
        u8 STS_MR : 1;
        /** Reserved */
        const u8 RSV0 : 7;
        /**
         * Transmit Message
         *
         * When set to ‘1’ by software, the HBA shall transmit the message
         * contained in the message buffer. When the message is completely sent,
         * the HBA shall clear this bit to ‘0’. A write of ‘0’ to this bit by
         * software shall have no effect. Software shall not change the contents
         * of the message buffer while CTL.TM is set to ‘1’.
         */
        u8 CTL_TM : 1;
        /**
         * Reset
         *
         * When set to ‘1’ by software, the HBA shall reset all enclosure
         * management message logic and the attached enclosure processor (if
         * applicable) and take all appropriate reset actions to ensure messages
         * can be transmitted/received after the reset. After the HBA completes
         * the reset operation, the HBA shall set the value to ‘0’. A write of
         * ‘0’ by software to this field shall have no effect.
         */
        u8 CTL_RST : 1;
        /** Reserved */
        const u8 RSV1 : 6;
        /**
         * LED Message Types
         *
         * If set to ‘1’, the HBA supports the LED message type defined in
         * section 12.2.1.
         */
        const u8 SUPP_LED : 1;
        /**
         * SAF-TE Enclosure Management Message
         *
         * If set to ‘1’, the HBA supports the SAF-TE message type.
         */
        const u8 SUPP_SAFTE : 1;
        /**
         * SES-2 Enclosure Management Message
         *
         * If set to ‘1’, the HBA supports the SES-2 message type.
         */
        const u8 SUPP_SES2 : 1;
        /**
         * SGPIO Enclosure Management Message
         *
         * If set to ‘1’, the HBA supports the SGPIO register interface message
         * type.
         */
        const u8 SUPP_SGPIO : 1;
        /** Reserved */
        const u8 RSV2 : 4;
        /**
         * Single Message Buffer
         *
         * If set to ‘1’, the HBA has one message buffer that is shared for
         * messages to transmit and messages received. In this case, unsolicited
         * receive messages are not supported and it is software’s
         * responsibility to manage access to this buffer. If cleared to ‘0’,
         * there are separate receive and transmit buffers such that unsolicited
         * messages could be supported.
         */
        const u8 ATTR_SMB : 1;
        /**
         * Transmit Only
         *
         * If set to ‘1’, the HBA only supports transmitting messages and does
         * not support receiving messages. If cleared to ‘0’, the HBA supports
         * transmitting and receiving messages.
         */
        const u8 ATTR_XMT : 1;
        /**
         * Activity LED Hardware Driven
         *
         * If set to ‘1’, the HBA drives the activity LED for the LED message
         * type in hardware and does not utilize software settings for this LED.
         * The HBA does not begin transmitting the hardware based activity
         * signal until after software has written CTL.TM=1 after a reset
         * condition.
         */
        const u8 ATTR_ALHD : 1;
        /**
         * Port Multiplier Support
         *
         * If set to ‘1’, the HBA supports enclosure management messages for
         * devices attached via a Port Multiplier. If cleared to ‘0’, the HBA
         * does not support enclosure management messages for devices attached
         * via a Port Multiplier. When cleared to ‘0’, software should use the
         * Serial ATA enclosure management bridge that is built into many Port
         * Multipliers for enclosure services with these devices. For more
         * information on Serial ATA enclosure management bridges, refer to the
         * Serial ATA Revision 2.6 specification.
         */
        const u8 ATTR_PM : 1;
        /** Reserved */
        const u8 RSV3 : 4;
    } __attribute__((packed));

    struct ECAP_T
    {
        /** BIOS/OS Handoff */
        const u8 BIOS_OS_Handoff : 1;
        /** NVMHCI Present */
        const u8 NVMP : 1;
        /** Automatic Partial to Slumber Transition */
        const u8 APST : 1;
        /** Supports Device Sleep */
        const u8 SDS : 1;
        /** Supports Aggressive Device Sleep Management */
        const u8 SADM : 1;
        /**
         * DevSleep Entrance from Slumber Only
         *
         * This field specifies that the HBA shall only assert the DEVSLP signal
         * if the interface is in Slumber. When this bit is set to ‘1’, the HBA
         * shall ingnore software directed entrance to DevSleep via PxCMD.ICC
         * unless PxSSTS.IPM = 6h. When this bit is cleared to ‘0’, the HBA may
         * enter DevSleep from any link state (active, Partial, or Slumber).
         */
        const u8 DESO : 1;
        /** Reserved */
        const u32 RSV0 : 26;
    } __attribute__((packed));

    /**
     * This register controls various global actions of the HBA. This register
     * is not affected by an HBA reset.
     */
    struct BOHC_T
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
        u32 Reserved0 : 27;
    } __attribute__((packed));

    struct InterruptStatusT
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
        const u16 Reserved0 : 14;
        /** PhyRdy Change Status */
        const u8 PRCS : 1;
        /** Incorrect Port Multiplier Status */
        u8 IPMS : 1;
        /** Overflow Status */
        u8 OFS : 1;
        /** Reserved */
        const u8 Reserved1 : 1;
        /** Interface Non-fatal Error Status */
        u8 INFS : 1;
        /** Interface Fatal Error Status */
        u8 IFS : 1;
        /** Host Bus Data Error Status */
        u8 HBDS : 1;
        /** Host Bus Fatal Error Status */
        u8 HBFS : 1;
        /** Task File Error Status */
        u8 TaskFileErrorStatus : 1;
        /** Cold Port Detect Status */
        u8 CPDS : 1;
    } __attribute__((packed));

    /**
     * This register enables and disables the reporting of the corresponding
     * interrupt to system software. When a bit is set (‘1’) and the
     * corresponding interrupt condition is active, then an interrupt is
     * generated. Interrupt sources that are disabled (‘0’) are still reflected
     * in the status registers. This register is symmetrical with the PxIS
     * register.
     */
    struct InterruptEnableT
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
        const u16 Reserved0 : 14;
        /** PhyRdy Change Interrupt Enable */
        u8 PRCE : 1;
        /** Incorrect Port Multiplier Enable */
        u8 IPME : 1;
        /** Overflow Enable */
        u8 OFE : 1;
        /** Reserved */
        const u8 Reserved1 : 1;
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

    struct CommandT
    {
        /** Start */
        u8 Start : 1;
        /** Spin-Up Device */
        u8 SpinUpDevice : 1;
        /** Power On Device */
        u8 PowerOnDevice : 1;
        /** Command List Override */
        u8 CommandListOverride : 1;
        /** FIS Receive Enable */
        u8 FISReceiveEnable : 1;
        /** Reserved */
        const u8 Reserved0 : 3;
        /** Current Command Slot */
        const u8 CurrentCommandSlot : 5;
        /** Mechanical Presence Switch State */
        const u8 MechanicalPresenceSwitchState : 1;
        /** FIS Receive Running */
        const u8 FISReceiveRunning : 1;
        /** Command List Running */
        const u8 CommandListRunning : 1;
        /** Cold Presence State */
        const u8 ColdPresenceState : 1;
        /** Port Multiplier Attached */
        u8 PortMultiplierAttached : 1;
        /** Hot Plug Capable Port */
        const u8 HotPlugCapablePort : 1;
        /** Mechanical Presence Switch Attached to Port */
        const u8 MechanicalPresenceSwitchAttached : 1;
        /** Cold Presence Detection */
        const u8 ColdPresenceDetection : 1;
        /** External SATA Port */
        const u8 ExternalSATAPort : 1;
        /** FIS-based Switching Capable Port */
        const u8 FISBasedSwitchingCapable : 1;
        /** Automatic Partial to Slumber Transitions Enable */
        u8 AutomaticPartialSlumberTransitionEnable : 1;
        /** Device is ATAPI */
        u8 ATAPI : 1;
        /** Drive LED on ATAPI Enable */
        u8 DriveLEDEnable : 1;
        /** Aggressive Link Power Management Enable */
        u8 AggressiveLinkPowerManagementEnable : 1;
        /** Aggressive Slumber / Partial */
        u8 AggressiveSlumber_Partial : 1;
        /** Interface Communication Control */
        ICC_E InterfaceCommunicationControl : 4;
    } __attribute__((packed));

    struct TaskFileDataStatusT
    {
        /** Indicates an error during the transfer. */
        u8 Error : 1;
        /** */
        u8 Index : 1;
        /** */
        u8 CorrectedData : 1;
        /** Indicates a data transfer is requested */
        u8 DataTransferRequest : 1;
        /** */
        u8 SeekComplete : 1;
        /** */
        u8 WriteFault : 1;
        /** */
        u8 Ready : 1;
        /** Indicates the interface is busy */
        u8 Busy : 1;
    } __attribute__((packed));

    struct TaskFileDataErrorT
    {
        u8 ErrorCode : 3;
        u8 Info      : 5;
    } __attribute__((packed));

    /**
     * This is a 32-bit register that copies specific fields of the task file
     * when FISes are received. The FISes that contain this information are:
     * - D2H Register FIS
     * - PIO Setup FIS
     * - Set Device Bits FIS (BSY and DRQ are not updated with this FIS)
     */
    struct TaskFieldDataT
    {
        /** Status */
        TaskFileDataStatusT Status;
        /** Error */
        TaskFileDataErrorT Error;
        /** Reserved */
        const u8 Reserved0[2];
    } __attribute__((packed));

    struct SignatureT
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

    /**
     * This 32-bit register conveys the current state of the interface and host.
     * The HBA updates it continuously and asynchronously. When the HBA
     * transmits a COMRESET to the device, this register is updated to its reset
     * values.
     */
    struct SATA_StatusT
    {
        /** Device Detection */
        const u8 DeviceDetection : 4;
        /** Current Interface Speed */
        const u8 Speed : 4;
        /** Interface Power Management */
        const u8 InterfacePowerManagement : 4;
        /** Reserved */
        const u32 Reserved0 : 20;
    } __attribute__((packed));

    /**
     * This is a 32-bit read-write register by which software controls SATA
     * capabilities. Writes to this register result in an action being taken by
     * the host adapter or interface. Reads from the register return the last
     * value written to it.
     */
    struct SATA_ControlT
    {
        /** Device Detection Initialization */
        u8 DeviceDetection : 4;
        /** Speed Allowed */
        u8 Speed : 4;
        /** Interface Power Management Transition Allowed */
        u8 InterfacePowerManagement : 4;
        /** Select Power Management */
        const u8 PowerManagement : 4;
        /** Port Multiplier Port */
        const u8 PortMulitplierPort : 4;
        /** Reserved */
        const u16 Reserved0 : 12;
    } __attribute__((packed));

    struct SATA_ErrorT
    {
        /** Error */
        u16 Error;
        /** Diagnostics */
        u16 Diagnostics;
    } __attribute__((packed));

    /**
     * This register is used to control and obtain status for Port Multiplier
     * FIS-based switching.
     */
    struct FIS_BasedSwitchingT
    {
        /** Enable */
        u8 Enable : 1;
        /** Device Error Clear */
        u8 DeviceErrorClear : 1;
        /** Single Device Error */
        const u8 SingleDeviceError : 1;
        /** Reserved */
        const u8 Reserved0 : 5;
        /** Device to Issue */
        u8 Device : 4;
        /** Active Device Optimization */
        const u8 ActiveDeviceOptimization : 4;
        /** Device with Error */
        const u8 DeviceWithError : 4;
        /** Reserved */
        const u16 Reserved1 : 12;
    } __attribute__((packed));

    struct DeviceSleepT
    {
        /** Aggressive Device Sleep Enable */
        u8 AggressiveDeviceSleepEnable : 1;
        /** Device Sleep Present */
        const u8 DeviceSleepPresent : 1;
        /** Device Sleep Exit Timeout */
        u8 DeviceSleepExitTimeout : 8;
        /** Minimum Device Sleep Assertion Time */
        u8 MinimumDeviceSleepAssertionTime : 5;
        /** DITO - Device Sleep Idle Timeout */
        u16 DeviceSleepIdleTimeout : 10;
        /** DITO Multiplier */
        const u8 DITOMuliplier : 4;
        /** Reserved */
        const u8 Reserved0 : 3;
    } __attribute__((packed));

    struct PortT
    {
        /** Command List Base Address (1K) */
        u64 CommandListBase;
        /** FIS Base Address (1K) */
        u64 FISBase;
        /** Interrupt Status */
        InterruptStatusT InterruptStatus;
        /** Interrupt Enable */
        InterruptEnableT InterruptEnable;
        /** Command and Status */
        CommandT Command_Status;
        /** Reserved */
        u8 Reserved0[4];
        /** Task File Data */
        TaskFieldDataT TaskFileData;
        /** Signature */
        SignatureT Signature;
        /** SATA Status */
        SATA_StatusT SATAStatus;
        /** SATA Control */
        SATA_ControlT SATAControl;
        /** SATA Error */
        SATA_ErrorT SATAError;
        /** SATA Active */
        u32 SATAActive;
        /** Command Issue */
        u32 CommandIssue;
        /** SATA Notification */
        u32 SATANotification;
        /** FIS-based Switch Control */
        FIS_BasedSwitchingT FISBasedSwitchControl;
        /** Device Sleep */
        DeviceSleepT DeviceSleep;
        /** Reserved */
        u8 Reserved1[0x70 - 0x48];
        /** Vendor Specific */
        u8 Vendor[0x80 - 0x70];
    } __attribute__((packed));

    /**
     * The memory mapped registers within the HBA exist in non-cacheable memory
     * space. Additionally, locked accesses are not supported. If software
     * attempts to perform locked transactions to the registers, indeterminate
     * results may occur. Register accesses shall have a maximum size of
     * 64-bits; 64-bit access must not cross an 8-byte alignment boundary. The
     * registers are broken into two sections – global registers and port
     * control. All registers that start below address 100h are global and meant
     * to apply to the entire HBA. The port control registers are the same for
     * all ports, and there are as many register banks as there are ports. All
     * registers not defined and all reserved bits within registers return ‘0’
     * when read.
     */
    struct MEM_T
    {
        /**
         * HBA Capabilities
         *
         * This register indicates basic capabilities of the HBA to driver
         * software.
         */
        CAP_T CAP;
        /**
         * Global HBA Control
         *
         * This register controls various global actions of the HBA.
         */
        GHC_T GHC;
        /**
         * Interrupt Status Register
         *
         * This register indicates which of the ports within the controller have
         * an interrupt pending and require service.
         *
         * If set, indicates that the corresponding port has an interrupt
         * pending. Software can use this information to determine which ports
         * require service after an interrupt.
         *
         * The IPS[x] bit is only defined for ports that are implemented or for
         * the command completion coalescing interrupt defined by CCC_CTL.INT.
         * All other bits are reserved.
         */
        u32 IS;
        /**
         * Ports Implemented
         *
         * This register indicates which ports are exposed by the HBA. It is
         * loaded by the BIOS. It indicates which ports that the HBA supports
         * are available for software to use. For example, on an HBA that
         * supports 6 ports as indicated in CAP.NP, only ports 1 and 3 could be
         * available, with ports 0, 2, 4, and 5 being unavailable.
         *
         * Software must not read or write to registers within unavailable
         * ports.
         *
         * The intent of this register is to allow system vendors to build
         * platforms that support less than the full number of ports implemented
         * on the HBA silicon.
         *
         * This register is bit significant. If a bit is set to ‘1’, the
         * corresponding port is available for software to use. If a bit is
         * cleared to ‘0’, the port is not available for software to use. The
         * maximum number of bits set to ‘1’ shall not exceed CAP.NP + 1,
         * although the number of bits set in this register may be fewer than
         * CAP.NP + 1. At least one bit shall be set to ‘1’.
         */
        const u32 PI;
        /**
         * Version
         *
         * This register indicates the major and minor version of the AHCI
         * specification that the HBA implementation supports. The upper two
         * bytes represent the major version number, and the lower two bytes
         * represent the minor version number. Example: Version 3.12 would be
         * represented as 00030102h. Three versions of the specification are
         * valid: 0.95, 1.0, 1.1, 1.2, 1.3, and 1.3.1
         */
        u32 VS;
        /**
         * Command Completion Coalescing Control
         *
         * The command completion coalescing control register is used to
         * configure the command completion coalescing feature for the entire
         * HBA.
         *
         * Implementation Note: HBA state variables (examples include
         * hCccComplete and hCccTimer) are used to describe the required
         * externally visible behavior. Implementations are not required to have
         * internal state values that directly correspond to these variables.
         */
        CCC_CTL_T CCC_CTL;
        /**
         * Command Completion Coalescing Ports
         *
         * The command completion coalescing ports register is used to specify
         * the ports that are coalesced as part of the CCC feature when
         * CCC_CTL.EN = ‘1’.
         *
         * This register is bit significant. Each bit corresponds to a
         * particular port, where bit 0 corresponds to port 0. If a bit is set
         * to ‘1’, the corresponding port is part of the command completion
         * coalescing feature. If a bit is cleared to ‘0’, the port is not part
         * of the command completion coalescing feature. Bits set to ‘1’ in this
         * register must also have the corresponding bit set to ‘1’ in the Ports
         * Implemented register. An updated value for this field shall take
         * effect within one timer increment (1 millisecond).
         */
        u32 CCC_PORTS;
        /**
         * Enclosure Management Location
         *
         * The enclosure management location register identifies the location
         * and size of the enclosure management message buffer.
         */
        EM_LOC_T EM_LOC;
        /**
         * Enclosure Management Control
         *
         * This register is used to control and obtain status for the enclosure
         * management interface. The register includes information on the
         * attributes of the implementation, enclosure management messages
         * supported, the status of the interface, whether any messages are
         * pending, and is used to initiate sending messages.
         */
        EM_CTL_T EM_CTL;
        /**
         * Host Capabilities Extended
         *
         * This register indicates capabilities of the HBA to driver software.
         */
        ECAP_T ECAP;
        /** BIOS/OS Handoff Control / Status */
        BOHC_T BOHC;

        /** Reserved */
        u8 RSV0[0x60 - 0x2C];
        /** Reserved for NVMHCI */
        u8 RSV1[0xA0 - 0x60];

        /** Vendor Specific Registers */
        u8 VSR[0x100 - 0xA0];

        /** Port Control Registers */
        PortT PCR[32];
    } __attribute__((packed));

    struct CommandHeaderT
    {
        /** Command FIS Length */
        u8 Command_FIS_Length : 5;
        /** ATAPI */
        u8 ATAPI : 1;
        /** Write */
        u8 Write : 1;
        /** Prefetchable */
        u8 Prefetchable : 1;
        /** Reset */
        u8 Reset : 1;
        /** BIST */
        u8 BIST : 1;
        /** Clear busy upon R_OK */
        u8 Clear : 1;
        /** Reserved */
        u8 Reserved0 : 1;
        /** Port Multiplier Port */
        u8 PortMultiplierPort : 4;
        /** Physical Region Descriptor Table Length */
        u16 PRDTableLength;

        /** Physical Region Descriptor Byte Count */
        u32 PRDByteCount;

        /** Command Table Descriptor Base Address */
        u64 CommandTableBase;

        /** Reserved */
        u8 Reserved1[16];
    } __attribute__((packed));

    /**
     * This table contains the scatter / gather list for the data transfer. It
     * contains a list of 0 (no data to transfer) to up to 65,535 entries. A
     * breakdown of each field in a PRD table is shown below. Item 0 refers to
     * the first entry in the PRD table. Item "CH[PRDTL] – 1" refers to the last
     * entry in the table, where the length field comes from the PRDTL field in
     * the command list entry for this command slot.
     */
    struct PRDTableEntryT
    {
        /** Data Base Address */
        u64 DataBaseAddress;
        /** Reserved */
        u8 Reserved0[4];

        /** Data Byte Count */
        u32 DataByteCount : 22;
        /** Reserved */
        u32 Reserved1 : 9;
        /** Interrupt on Completion */
        u32 Interrupt : 1;
    } __attribute__((packed));

    struct CommandTableT
    {
        /** Command FIS */
        u8 Command_FIS[64];

        /** ATAPI command, 12 or 16 bytes */
        u8 ATAPI_Command[16];

        /** Reserved */
        u8 Reserved0[48];

        /** Physical region descriptor table entries, 0 ~ 65535 */
        PRDTableEntryT PRDTable[1];
    } __attribute__((packed));

    struct FrameInformationStructureT
    {
        /** DMA Setup FIS */
        fis::DMA_SetupT DMA_Setup;
        u8 PAD0[0x04];

        /** PIO Setup FIS */
        fis::PIO_SetupT PIO_Setup;
        u8 PAD1[0x0C];

        /** Register - device to host FIS */
        fis::DeviceToHostT DeviceToHost;
        u8 PAD2[0x04];

        /** Set device bit FIS */
        fis::DeviceBitsT DeviceBits;

        u8 UFIS[0xA0 - 0x60];
        u8 Reserved0[0x100 - 0xA0];
    } __attribute__((packed));
}
