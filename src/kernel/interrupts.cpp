#include <scriptos/interrupts.hpp>
#include <scriptos/print.hpp>

__attribute__((interrupt)) void PageFault_Handler(interrupt_frame *frame)
{
    (void)frame;

    printf("page fault\n");
    for (;;)
        ;
}
