#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/nmi.hpp>
#include <scriptos/kernel/real.hpp>
#include <scriptos/kernel/cr.hpp>

void Jump_RealMode()
{
    /* disable interrupts */
    CLI();
    NMI::Disable();

    /* disable paging */
    auto cr0 = CR::CR0::R();
    cr0.PG = false;
    CR::CR0::W(cr0);
    CR::CR3::W(0);
}
