#include <libsystem/__plugs__.h>

#include <libruntime/SpinLock.h>

#include "arch/Arch.h"
#include "system/memory/Memory.h"

namespace hegel::plugs {

size_t get_page_size()
{
    return arch::get_page_size();
}

SpinLock _memory_lock;

bool memory_is_locked()
{
    return _memory_lock.is_acquired();
}

void memory_lock()
{
    _memory_lock.acquire();
}

void memory_unlock()
{
    _memory_lock.release();
}

ErrorOr<uintptr_t> memory_alloc(size_t page_count)
{
    return ErrorOr<uintptr_t>(hegel::memory::alloc_region(page_count).base_address());
}

void memory_free(uintptr_t addr, size_t page_count)
{
    hegel::memory::free_region(hegel::memory::MemoryRegion::from_aligned_address(addr, page_count));
}

}