#include "System.h"

#include <libsystem/Logger.h>
#include <libutil/Move.h>
#include <stdarg.h>

#include <arch/Arch.h>

#ifndef PANIC_CALLBACK_COUNT
#define PANIC_CALLBACK_COUNT 16
#endif

namespace hegel {

static uint64_t _current_tick;

struct panic_dump_t {
    panic_dump_callback_t func;
    void* userdata;
};

static panic_dump_t _panic_dump_callbacks[PANIC_CALLBACK_COUNT] = {
    { .func = nullptr, .userdata = nullptr }
};

void PANIC(const char* file, int line, const char* message, ...) 
{
    hegel::plugs::out_stream = move(hegel::plugs::log_stream);
    hegel::plugs::err_stream = move(hegel::plugs::log_stream);

    logger_fatal("KERNEL PANIC BEGIN");
    fprintf((FILE*)&hegel::plugs::log_stream, "At %s:%d\n", file, line);
    va_list va;
    va_start(va, message);
    vfprintf((FILE*)&hegel::plugs::log_stream, message, va);
    va_end(va);

    for(size_t i = 0; i < PANIC_CALLBACK_COUNT; ++i) {
        if(_panic_dump_callbacks[i].func == nullptr) break;
            _panic_dump_callbacks[i].func(_panic_dump_callbacks[i].userdata);
    }

    logger_fatal("KERNEL PANIC END");

    arch::stop();
}

void register_panic_dump_callback(panic_dump_callback_t func, void* userdata)
{
    size_t i;
    for(i = 0; i < PANIC_CALLBACK_COUNT; ++i)
    {
        if(_panic_dump_callbacks[i].func == nullptr) {
            _panic_dump_callbacks[i].func = func;
            _panic_dump_callbacks[i].userdata = userdata;
            i++;
            if(i != PANIC_CALLBACK_COUNT) {
                _panic_dump_callbacks[i].func = nullptr;
            }
            break;
        }
    }
}

void system_tick()
{
    if(_current_tick + 1 < _current_tick) {
        panic("System tick overflow at %u\n", _current_tick);
    }

    __atomic_add_fetch(&_current_tick, 1, __ATOMIC_SEQ_CST);
}

uint64_t get_tick()
{
    return __atomic_load_n(&_current_tick, __ATOMIC_SEQ_CST);
}

void system_hang() {
    while(true) { arch::halt(); }
}

}