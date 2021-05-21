#pragma once

#include <libsystem/Macros.h>
#include <libsystem/Types.h>

#include <kernel/bootdata/Bootdata.h>

namespace hegel {

void PANIC(const char* message) __noreturn;
void tick();
uint64_t get_tick();

void system_main(const boot::Bootdata* bootdata) __noreturn;

}