#include "MemoryRange.h"

#include <arch/Arch.h>

namespace memory {

size_t MemoryRange::page_count()
{
    return _size / ARCH_PAGE_SIZE;
}

bool MemoryRange::is_page_aligned()
{
    return IS_PAGE_ALIGN(base()) && IS_PAGE_ALIGN(size());
}

MemoryRange MemoryRange::from_non_aligned_address(uintptr_t base, size_t size)
{
    size_t align = ARCH_PAGE_SIZE - base % ARCH_PAGE_SIZE;

    if(base % ARCH_PAGE_SIZE == 0) {
        align = 0;
    }

    base += align;
    size -= align;

    size = __align_down(size, ARCH_PAGE_SIZE);

    return (MemoryRange){base, size};
}

MemoryRange MemoryRange::around_non_aligned_address(uintptr_t base, size_t size)
{
    size_t align = base % ARCH_PAGE_SIZE;

    base -= align;
    size += align;

    size = __align_up(size, ARCH_PAGE_SIZE);

    return (MemoryRange){base, size};
}

}