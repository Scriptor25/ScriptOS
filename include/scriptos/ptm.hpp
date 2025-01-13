#pragma once

#include <scriptos/paging.hpp>
#include <scriptos/types.hpp>

class PageTableManager
{
public:
    static PageTableManager &GetKernelInstance();

    void Init(PageDirectoryEntry *page_directory);

    void MapPage(void *virtual_address, void *physical_address);
    void MapPages(void *virtual_address, void *physical_address, usize count);

    void SetupPaging();

private:
    PageDirectoryEntry *m_PageDirectory;
};
