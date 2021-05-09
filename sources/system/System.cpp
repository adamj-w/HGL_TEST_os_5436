#include <system/System.h>

#include <libsystem/Logger.h>
#include <arch/Arch.h>

namespace hegel {

void PANIC(const char* message) 
{
    logger_fatal("KERNEL PANIC: {}", message);

    arch::stop();
}

}