#pragma once

#include <libruntime/Types.h>

namespace hegel::memory {

// TODO: make formattable and finish class
class MemoryRegion
{
private:
    uintptr_t _base_page;
    size_t _page_count;

    MemoryRegion(uintptr_t base_page, size_t page_count);

public:
    MemoryRegion();
    virtual ~MemoryRegion();

    static MemoryRegion from_aligned_address(uintptr_t addr, size_t size);

    uintptr_t base_address();
};

}