#include <scriptos/kernel/graphics.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/std/util.hpp>
#include <scriptos/stl/string.hpp>
#include <scriptos/stl/vector.hpp>

void printg(u32 rows, u32 columns, u32 maxw, const cstr *data)
{
    auto minw = reinterpret_cast<u32 *>(malloc(columns * sizeof(u32)));
    auto minh = reinterpret_cast<u32 *>(malloc(rows * sizeof(u32)));

    memset(minw, 0, columns * sizeof(u32));
    memset(minh, 0, rows * sizeof(u32));

    if (data)
        for (u32 r = 0; r < rows; ++r)
            for (u32 c = 0; c < columns; ++c)
            {
                auto s = data[r * columns + c];
                auto l = static_cast<u32>(strlen(s));
                auto w = maxw ? min(l, maxw) : l;
                auto h = (l <= w) ? 1 : ceil_div(l, w);
                minw[c] = max(minw[c], w);
                minh[r] = max(minh[r], h);
            }

    u32 tblw = 0;
    for (u32 c = 0; c < columns; ++c)
        tblw += (minw[c] + 1) * CHAR_W;

    u32 tblh = 0;
    for (u32 r = 0; r < rows; ++r)
        tblh += (minh[r] + 1) * CHAR_H;

    auto &graphics = Graphics::GetKernelInstance();
    auto &pos = graphics.Pos();

    auto x = pos.x + CHAR_W;
    auto y = pos.y + CHAR_H;

    for (u32 r = 0; r < rows; ++r)
    {
        for (u32 c = 0; c < columns; ++c)
        {
            graphics.DrawString(x, y, maxw * CHAR_W, data[r * columns + c]);

            x += (minw[c] + 1) * CHAR_W;
        }
        y += (minh[r] + 1) * CHAR_H;
        x = pos.x + CHAR_W;
    }

    x = pos.x + CHAR_W2;
    y = pos.y + CHAR_H2;

    for (u32 r = 0; r < rows; ++r)
    {
        graphics.DrawLine(x, y, x + tblw, y);
        y += (minh[r] + 1) * CHAR_H;
    }
    graphics.DrawLine(x, y, x + tblw, y);

    x = pos.x + CHAR_W2;
    y = pos.y + CHAR_H2;

    for (u32 c = 0; c < columns; ++c)
    {
        graphics.DrawLine(x, y, x, y + tblh);
        x += (minw[c] + 1) * CHAR_W;
    }
    graphics.DrawLine(x, y, x, y + tblh);

    pos.y += tblh + CHAR_H;

    free(minw);
    free(minh);
}

struct tbl_t
{
    u32 columns;
    u32 maxw;
    vector<string> data;
} static global_tbl;

void beg_tbl(u32 columns, u32 maxw, ...)
{
    global_tbl.columns = columns;
    global_tbl.maxw = maxw;
    global_tbl.data = vector<string>();

    va_list ap;
    va_start(ap, maxw);

    for (u32 c = 0; c < columns; ++c)
    {
        global_tbl.data.emplace_back(va_arg(ap, cstr));
        global_tbl.data.back().push_back(0);
    }

    va_end(ap);
}

void end_tbl()
{
    vector<cstr> raw_data;
    for (auto &s : global_tbl.data)
        raw_data.push_back(s.data());

    printg(global_tbl.data.size() / global_tbl.columns, global_tbl.columns, global_tbl.maxw, raw_data.data());
}

void next_col(cstr data)
{
    global_tbl.data.emplace_back(data);
    global_tbl.data.back().push_back(0);
}
