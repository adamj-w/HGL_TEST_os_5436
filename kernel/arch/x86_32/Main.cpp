#include "device/SerialStream.h"
#include "device/CGATerminal.h"
#include "device/PIT.h"
#include "segmentation/Segmentation.h"
#include "interrupts/Interrupts.h"
#include "acpi/ACPI.h"
#include "smbios/SMBIOS.h"
#include "CPUID.h"

#include <kernel/system/System.h>
#include <kernel/bootdata/Multiboot2.h>
#include <kernel/bootdata/Bootdata.h>
#include <kernel/memory/Memory.h>

#include <libsystem/__plugs__.h>
#include <libsystem/Logger.h>
#include <libsystem/RefPtr.h>

#include <arch/Arch.h>

using namespace hegel::arch;
using namespace hegel::arch::x86;

namespace hegel {

extern "C" void arch_main(uint32_t multiboot_magic, uintptr_t multiboot_addr)
{
    auto serial = SerialStream(SerialPort::COM1);
    __plug_init_libsystem(&serial);

    if(!cpuid_supported()) logger_fatal("System doesn't support CPUID.");
    cpuid_check_system_requirements();
    
    logger_info("Booting...");
    logger_info("Hegel Kernel (%s %s)", __BUILD_TARGET__, __BUILD_GITREF__);
    logger_info("Kernel built on \"%s\"", __BUILD_UNAME__);
    logger_info("System running on \"%s\"", cpuid_vendor_name());

    auto multiboot = boot::Multiboot(multiboot_addr, multiboot_magic);

    if(!multiboot.is_valid()) {
        logger_fatal("Invalid bootloader with magic %#010X, please boot with multiboot2 specification", multiboot_magic);
        PANIC("Invalid bootloader, please try with valid multiboot2 loader.\n");
    } else {
        logger_info("Found valid bootloader with name \"%s\"", multiboot.bootloader());
    }
    
    auto* bootdata = multiboot.get_bootdata();

    segmentation_initialize();
    interrupts_initialize();

    acpi::initialize(bootdata);
    smbios::initialize({0xF0000, 0xFFFF});

    system_main(bootdata);

    arch::stop();
}

}