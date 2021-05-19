#pragma once

#include <libsystem/Macros.h>
#include <libsystem/Types.h>

namespace hegel {

void PANIC(const char* message) __noreturn;
void tick();
uint64_t get_tick();

}