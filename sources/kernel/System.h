#pragma once

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace hegel {

void PANIC(const char* message) __noreturn;
void tick();
uint64_t get_tick();

}