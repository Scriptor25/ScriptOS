#include <multiboot2.h>
#include <scriptos/io.h>
#include <scriptos/types.h>

void main(u64 magic, u64 addr)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || (addr & 7))
        return;
}
