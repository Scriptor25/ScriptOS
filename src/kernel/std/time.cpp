#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pit.hpp>
#include <scriptos/std/time.hpp>

void sleep(u32 millis)
{
    auto end = PIT::TicksSinceBoot + (PIT_TICKS_PER_SECOND * millis) / 1000;
    while (PIT::TicksSinceBoot < end)
        HLT();
}
