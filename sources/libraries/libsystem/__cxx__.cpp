#include <libruntime/Macros.h>
#include <libruntime/Types.h>
#include <libruntime/Assert.h>
#include <libsystem/__alloc__.h>

using namespace hegel;

void* operator new(size_t size)
{
    return __alloc__::malloc(size - size % 4 + 4);
}

void operator delete(void* ptr)
{
    __alloc__::free(ptr);
}
