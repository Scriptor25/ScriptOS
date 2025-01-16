#pragma once

#include <scriptos/std/types.hpp>

struct interrupt_frame
{
    void *IP;
    u32 CS;
    u32 EFLAGS;
    u32 ESP;
    u32 SS;
};

void DE_Handler(interrupt_frame *frame);
void DB_Handler(interrupt_frame *frame);
void BP_Handler(interrupt_frame *frame);
void OF_Handler(interrupt_frame *frame);
void BR_Handler(interrupt_frame *frame);
void UD_Handler(interrupt_frame *frame);
void NM_Handler(interrupt_frame *frame);
void DF_Handler(interrupt_frame *frame, u32 code);
void TS_Handler(interrupt_frame *frame, u32 code);
void NP_Handler(interrupt_frame *frame, u32 code);
void SS_Handler(interrupt_frame *frame, u32 code);
void GP_Handler(interrupt_frame *frame, u32 code);
void PF_Handler(interrupt_frame *frame, u32 code);
void MF_Handler(interrupt_frame *frame);
void AC_Handler(interrupt_frame *frame, u32 code);
void MC_Handler(interrupt_frame *frame);
void XM_XF_Handler(interrupt_frame *frame);
void VE_Handler(interrupt_frame *frame);
void CP_Handler(interrupt_frame *frame, u32 code);
void HV_Handler(interrupt_frame *frame);
void VC_Handler(interrupt_frame *frame, u32 code);
void SX_Handler(interrupt_frame *frame, u32 code);

void PIT_Handler(interrupt_frame *frame);

void SYS_Handler(interrupt_frame *frame);
