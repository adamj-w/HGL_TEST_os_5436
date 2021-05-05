#pragma once

#include <libruntime/Macros.h>
#include <libruntime/RefPtr.h>
#include <libruntime/Types.h>

namespace hegel {
namespace arch {

void stop() __noreturn;

void idle() __noreturn;

void halt();

void yield();

size_t get_page_size();

}
}