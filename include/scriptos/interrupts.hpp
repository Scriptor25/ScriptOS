#pragma once

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame *frame);
