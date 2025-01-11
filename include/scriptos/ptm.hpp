#pragma once

#include <scriptos/paging.hpp>
#include <scriptos/types.hpp>

struct PageIndex
{
    explicit PageIndex(uptr virtual_address);

    uptr PDI;
    uptr PTI;
};

class PageTableManager
{
public:
    PageTableManager();
    explicit PageTableManager(PageDirectoryEntry *pd);

    void MapPage(void *virtual_address, void *physical_address);
    void MapPages(void *virtual_address, void *physical_address, usize count);

    void SetupPaging();

private:
    PageDirectoryEntry *m_PD;
};
