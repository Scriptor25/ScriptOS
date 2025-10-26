#include <scriptos/print.h>

#define CHUNK_SIZE 0x10

void fkprintmem(
    out_stream stream,
    const void* buffer,
    usize buffer_length)
{
    for (usize n = 0; n < buffer_length; n += CHUNK_SIZE)
    {
        auto p = reinterpret_cast<const u8*>(buffer) + n;

        fkprintf(stream, "%016x | ", p);
        for (usize d = 0; d < CHUNK_SIZE; ++d)
        {
            fkprintf(stream, "%02X ", p[d]);
        }

        fkprintf(stream, "| ");
        for (usize d = 0; d < CHUNK_SIZE; ++d)
        {
            auto c = p[d];
            fkprintf(stream, "%c", c < 0x20 ? '.' : c);
        }

        fkprintf(stream, "\r\n");
    }
}
