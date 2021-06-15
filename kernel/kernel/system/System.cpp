#include "System.h"

#include <libsystem/Logger.h>
#include <stdarg.h>

#include <arch/Arch.h>

namespace hegel {

static uint64_t _current_tick;

void PANIC(const char* message, ...) 
{
    logger_fatal("KERNEL PANIC BEGIN");
    va_list va;
    va_start(va, message);
    vfprintf((FILE*)&hegel::plugs::log_stream, message, va);
    va_end(va);
    logger_fatal("KERNEL PANIC END");

    //__alloc__::liballoc_dump();

    arch::stop();
}

void system_tick()
{
    if(_current_tick + 1 < _current_tick) {
        PANIC("System tick overflow at %u\n", _current_tick);
    }

    __atomic_add_fetch(&_current_tick, 1, __ATOMIC_SEQ_CST);
}

uint64_t get_tick()
{
    return __atomic_load_n(&_current_tick, __ATOMIC_SEQ_CST);
}

}