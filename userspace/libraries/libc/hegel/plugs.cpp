#include <libsystem/__plugs__.h>
#include <libsystem/Stdio.h>

#include <stdlib.h>
#include <stdio.h>

namespace hegel {

RefPtr<Stream> stdin;
RefPtr<Stream> stdout;
RefPtr<Stream> stderr;

bool memory_is_locked();
void memory_lock();
void memory_unlock();
size_t get_page_size();
ErrorOr<uintptr_t> memory_alloc(size_t page_count);
void memory_free(uintptr_t addr, size_t page_count);

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

}