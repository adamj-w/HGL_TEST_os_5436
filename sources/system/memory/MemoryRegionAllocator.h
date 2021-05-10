#pragma once

#include "MemoryRegion.h"
#include "MemoryPool.h"

#include <arch/Arch.h>

namespace hegel::memory {

class MemoryRegionAllocator
{
private:
    MemoryPool _free_pool;
    MemoryPool _used_pool;

public:
    MemoryRegionAllocator() {}
    ~MemoryRegionAllocator() {}

    size_t used() const { return _used_pool.quantity() * arch::get_page_size(); }
    size_t free() const { return _free_pool.quantity() * arch::get_page_size(); }
    size_t total() const { return used() + free(); }

    MemoryRegion alloc_region(size_t page_count);
    void free_region(MemoryRegion region);
};

}