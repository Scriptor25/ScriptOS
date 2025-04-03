#include <scriptos/kernel/io.hpp>

void io_wait()
{
    out<u8>(0x80, 0);
}
