#include <system/memory/Memory.h>
#include <system/memory/MemoryRegionAllocator.h>
#include <system/System.h>

#include <libruntime/Assert.h>
#include <libsystem/Logger.h>

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
        PANIC("Either out of memory or allocated before parsing bootloader info!");
    }

    MemoryRegion region = MemoryRegion::empty();

    if(_bootstrap.page_count() >= page_count) {
        region = _bootstrap.take(page_count);

        if(_bootstrap.is_empty()) {
            logger_info("Used all the bootstrap memory");
        }
    } else {
        region = _allocator->alloc_region(page_count);
    }

    if(region.is_empty()) {
        PANIC("Failed to allocate. Out of memory!");
    }

    return region;
}

void free_region(MemoryRegion region) 
{
    auto kernel = arch::get_kernel_region();

    if(region.is_overlapping_with(kernel)) {
        MemoryRegion lower = region.half_under(kernel);

        if(!lower.is_empty()) {
            free_region(lower);
        }

        MemoryRegion upper = region.half_over(kernel);

        if(!upper.is_empty()) {
            free_region(lower);
        }
    } else if(!_bootstrapped) {
        logger_info("Bootstrapping with {}", region);

        _bootstrap = region;
        _bootstrapped = true;

        _allocator = new MemoryRegionAllocator();
    } else {
        _allocator->free_region(region);
    }
}

}