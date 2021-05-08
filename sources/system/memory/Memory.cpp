#include <system/memory/Memory.h>
#include <system/memory/MemoryRegionAllocator.h>

#include <libruntime/Assert.h>

namespace hegel::memory {

static bool _bootstrapped = false;
static MemoryRegion _bootstrap;
static MemoryRegionAllocator* _allocator;

bool is_bootstrapped() 
{
    return _bootstrapped;
}

MemoryRegion alloc_region(size_t page_count)
{
    assert(page_count > 0);

    if(!_bootstrapped) {
        // TODO: panic the kernel
    }

    MemoryRegion region = MemoryRegion::empty();

    if(_bootstrap.page_count() >= page_count) {
        region = _bootstrap.take(page_count);

        if(_bootstrap.is_empty()) {
            // TODO: info message about it being empty
        }
    } else {
        region = _allocator->alloc_region(page_count);
    }

    if(region.is_empty()) {
        // TODO: log out of memory
    }

    return region;
}

void free_region(MemoryRegion region) 
{
    __unused(region);

    // TODO: finish method
}

}