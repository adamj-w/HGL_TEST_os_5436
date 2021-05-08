#include <libc/stdint.h>

#include <arch/x86/device/SerialStream.h>

#include <libsystem/Stdio.h>
#include <libsystem/Logger.h>

using namespace hegel::arch;
using namespace hegel::arch::x86;

namespace hegel {

RefPtr<Stream> stdin;
RefPtr<Stream> stdout;
RefPtr<Stream> stderr;
RefPtr<Stream> stdlog;

extern "C" void arch_main(uint32_t multiboot_magic, uintptr_t multiboot_addr)
{
    auto serial = SerialStream(SerialPort::COM1);
    serial.make_orphan();

    stdout = adopt(serial);
    stderr = adopt(serial);
    stdlog = adopt(serial);

    logger_info("Booting...");
    logger_info("Hegel Kernel ({} {})", __BUILD_TARGET__, __BUILD_GITREF__);
    logger_info("Kernel built on {}", __BUILD_UNAME__);

    ((void)multiboot_magic);
    ((void)multiboot_addr);
}

}