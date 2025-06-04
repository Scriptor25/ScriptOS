#pragma once

#include <scriptos/types.h>

#define INTERRUPT __attribute__((interrupt))

namespace interrupt
{
    struct StackFrame
    {
        u64 IP;
        u64 CS;
        u64 Flags;
        u64 SP;
        u64 SS;
    } __attribute__((packed));

    void DE_Handler(StackFrame* stack_frame);
    void DB_Handler(StackFrame* stack_frame);
    void NI_Handler(StackFrame* stack_frame);
    void BP_Handler(StackFrame* stack_frame);
    void OF_Handler(StackFrame* stack_frame);
    void BR_Handler(StackFrame* stack_frame);
    void UD_Handler(StackFrame* stack_frame);
    void NM_Handler(StackFrame* stack_frame);
    void DF_Handler(StackFrame* stack_frame, u64 error_code);
    void TS_Handler(StackFrame* stack_frame, u64 error_code);
    void NP_Handler(StackFrame* stack_frame, u64 error_code);
    void SS_Handler(StackFrame* stack_frame, u64 error_code);
    void GP_Handler(StackFrame* stack_frame, u64 error_code);
    void PF_Handler(StackFrame* stack_frame, u64 error_code);
    void MF_Handler(StackFrame* stack_frame);
    void AC_Handler(StackFrame* stack_frame, u64 error_code);
    void MC_Handler(StackFrame* stack_frame);
    void XM_Handler(StackFrame* stack_frame);
    void VE_Handler(StackFrame* stack_frame);
    void CP_Handler(StackFrame* stack_frame, u64 error_code);
 
    void KE_Handler(StackFrame* stack_frame);
}
