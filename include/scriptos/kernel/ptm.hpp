#pragma once

#include <scriptos/kernel/paging.hpp>
#include <scriptos/std/types.hpp>

/**
 * Page Table Manager
 */
class PageTableManager
{
public:
    /**
     * Get the kernel managed instance of the page table manager.
     */
    static PageTableManager &GetKernelInstance();

    /**
     * Initialize this page table manager with a page directory.
     */
    void Initialize(PageDirectoryEntry *page_directory);

    /**
     * Map a single physical page to a virtual address for either user or kernel mode access.
     */
    bool MapPage(const void *virtual_address, const void *physical_address, bool user = false);
    /**
     * Map a range of physical pages to some virtual address space for either user or kernel mode access.
     */
    bool MapPages(const void *virtual_address, const void *physical_address, usize count, bool user = false);

    /**
     * Set up paging with this page table manager.
     */
    void SetupPaging();

    bool IsMapped(const void *virtual_address) const;

private:
    PageDirectoryEntry *m_PageDirectory;
};

/**
 * Invalidate a single page. Use this instead of updating the whole IDT to save some performance
 */
void InvalidatePage(const void *address);
