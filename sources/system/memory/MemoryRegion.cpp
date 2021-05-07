#include <system/memory/MemoryRegion.h>

namespace hegel::memory {

MemoryRegion::MemoryRegion()
{
    _base_page = 0;
    _page_count = 0;
}

MemoryRegion::MemoryRegion(uintptr_t base_page, size_t page_count)
{
    _base_page = base_page;
    _page_count = page_count;
}

MemoryRegion MemoryRegion::

}