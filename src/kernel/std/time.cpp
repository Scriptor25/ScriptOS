#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pit.hpp>
#include <scriptos/std/time.hpp>

void sleep(u32 millis)
{
    auto start = PIT::TicksSinceBoot;
    while (PIT::TicksSinceBoot < start + millis)
        HLT();
}
