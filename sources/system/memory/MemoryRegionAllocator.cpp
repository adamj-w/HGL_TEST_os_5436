#include <system/memory/MemoryRegionAllocator.h>

namespace hegel::memory {

MemoryRegion MemoryRegionAllocator::alloc_region(size_t page_count)
{
    auto region = _free_pool.take(page_count);
    _used_pool.put(region);
    return region;
}

void MemoryRegionAllocator::free_region(MemoryRegion region)
{
    _free_pool.put(region);
    _used_pool.take(region);
}

}