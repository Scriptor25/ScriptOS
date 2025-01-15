#pragma once

#include <scriptos/std/types.hpp>

struct interrupt_frame
{
    void *IP;
    u32 CS;
    u32 Flags;
};

__attribute__((interrupt)) void DE_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void DB_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void BP_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void OF_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void BR_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void UD_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void NM_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void DF_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void TS_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void NP_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void SS_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void GP_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void PF_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void MF_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void AC_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void MC_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void XM_XF_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void VE_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void CP_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void HV_Handler(interrupt_frame *frame);
__attribute__((interrupt)) void VC_Handler(interrupt_frame *frame, u32 code);
__attribute__((interrupt)) void SX_Handler(interrupt_frame *frame, u32 code);

__attribute__((interrupt)) void PIT_Handler(interrupt_frame *frame);
