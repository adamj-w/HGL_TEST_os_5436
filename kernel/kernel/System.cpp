#include "System.h"

#include <libsystem/Logger.h>
#include <arch/Arch.h>

namespace hegel {

static uint64_t _current_tick;

void PANIC(const char* message) 
{
    logger_fatal("KERNEL PANIC: %s", message);

    //__alloc__::liballoc_dump();

    arch::stop();
}

void tick()
{
    __atomic_add_fetch(&_current_tick, 1, __ATOMIC_SEQ_CST);
}

uint64_t get_tick()
{
    return __atomic_load_n(&_current_tick, __ATOMIC_SEQ_CST);
}

}