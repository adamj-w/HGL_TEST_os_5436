#include <system/System.h>

#include <libsystem/Logger.h>
#include <libsystem/__alloc__.h>
#include <arch/Arch.h>

namespace hegel {

void PANIC(const char* message) 
{
    logger_fatal("KERNEL PANIC: {}", message);

    __alloc__::liballoc_dump();

    arch::stop();
}

}