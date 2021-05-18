#include <libruntime/Macros.h>
#include <libruntime/Types.h>
#include <libruntime/Assert.h>
#include <libsystem/Logger.h>
#include <__alloc__.h>

using namespace hegel;

#define MAX_AT_EXIT_FUNCTION 128

struct AtExitFunctionEntry
{
    void (*destructor_func)(void *);
    void *obj_ptr;
    void *dso_handle;
};

static size_t __atexit_function_count = 0;
static AtExitFunctionEntry __atexit_function[MAX_AT_EXIT_FUNCTION];

void* __dso_handle;

extern "C" int __cxa_atexit(void (*f)(void*), void* objptr, void* dso)
{
    assert(__atexit_function_count < MAX_AT_EXIT_FUNCTION);

    __atexit_function[__atexit_function_count].destructor_func = f;
    __atexit_function[__atexit_function_count].obj_ptr = objptr;
    __atexit_function[__atexit_function_count].dso_handle = dso;
    __atexit_function_count++;

    return 0;
}

extern "C" void __cxa_pure_virtual()
{
    logger_error("Calling pure virtual function.");
    assert_not_reached();
}

void* operator new(size_t size)
{
    return __alloc__::malloc(size - size % 4 + 4);
}

void* operator new[](size_t size)
{
    return __alloc__::malloc(size - size % 4 + 4);
}

void operator delete(void* ptr)
{
    __alloc__::free(ptr);
}

void operator delete(void* ptr, unsigned long size)
{
    __unused(size);
    __alloc__::free(ptr);
}

void operator delete[](void* ptr) 
{
    __alloc__::free(ptr);
}

void operator delete[](void* ptr, unsigned long size)
{
    __unused(size);
    __alloc__::free(ptr);
}

