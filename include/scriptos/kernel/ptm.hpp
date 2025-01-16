#pragma once

#include <scriptos/kernel/paging.hpp>
#include <scriptos/std/types.hpp>

class PageTableManager
{
public:
    static PageTableManager &GetKernelInstance();

    void Init(PageDirectoryEntry *page_directory);

    void MapPage(void *virtual_address, void *physical_address, bool user = false);
    void MapPages(void *virtual_address, void *physical_address, usize count, bool user = false);

    void SetupPaging();

private:
    PageDirectoryEntry *m_PageDirectory;
};
