#include <libsystem/SpinLock.h>
#include <libsystem/Error.h>
#include <libsystem/__plugs__.h>

#include <stdlib.h>
#include <stdio.h>

namespace hegel::plugs {

Stream* in_stream;
Stream* out_stream;
Stream* err_stream;

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

size_t get_page_size()
{
    // TODO: work on after paging
    return 1024;
}

ResultOr<uintptr_t> memory_alloc(size_t page_count)
{
    __unused(page_count);
    // TODO: work on after paging
    return ResultOr<uintptr_t>(Result::NOT_IMPLEMENTED);
}

void memory_free(uintptr_t addr, size_t page_count)
{
    // TODO: work on after paging
    __unused(page_count);
    __unused(addr);
}

}

extern "C" void __assert_failed(const char* file, int line, const char* expr)
{
    printf("\"%s\" failed at %s:%d\n!", expr, file, line);
    exit(-1);
}

extern "C" void __assert_not_reach_reached(const char* file, int line)
{
    printf("Assertion reached at %s:%d\n!", file, line);
    exit(-1);
}