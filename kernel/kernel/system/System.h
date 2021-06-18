#pragma once

#include <libsystem/Macros.h>
#include <libsystem/Types.h>

#include <kernel/bootdata/Bootdata.h>

namespace hegel {

typedef void (*panic_dump_callback_t)(void* userdata);

void PANIC(const char* message, ...) __noreturn;
void register_panic_dump_callback(panic_dump_callback_t func, void* userdata);

void system_tick();
uint64_t get_tick();

void system_main(const boot::Bootdata* bootdata);
void system_initialize();

void system_hang() __noreturn;
void system_stop() __noreturn;

}