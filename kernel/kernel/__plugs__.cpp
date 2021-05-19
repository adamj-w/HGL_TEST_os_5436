#include <libsystem/__plugs__.h>

#include <libsystem/SpinLock.h>

#include "arch/Arch.h"
#include "System.h"
#include "memory/Memory.h"

namespace hegel::plugs {

Stream* in_stream = nullptr;
Stream* out_stream = nullptr;
Stream* err_stream = nullptr;
Stream* log_stream = nullptr;

extern "C" void __plug_init_libsystem(hegel::Stream* stream)
{
    out_stream = stream;
    err_stream = stream;
    log_stream = stream;
}

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

void assert_failed() 
{
    PANIC("Assertion failed in kernel.");
}

void assert_not_reached_reached() 
{
    PANIC("Reached unreachable assertion in kernel.");
}

}