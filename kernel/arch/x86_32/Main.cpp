#include "device/SerialStream.h"
#include "device/CGATerminal.h"
#include "device/PIT.h"
#include "segmentation/Segmentation.h"
#include "interrupts/Interrupts.h"
#include "acpi/ACPI.h"
#include "smbios/SMBIOS.h"

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

    logger_info("Booting...");
    logger_info("Hegel Kernel (%s %s)", __BUILD_TARGET__, __BUILD_GITREF__);
    logger_info("Kernel built on \"%s\"", __BUILD_UNAME__);

    auto multiboot = boot::Multiboot(multiboot_addr, multiboot_magic);

    if(!multiboot.is_valid()) {
        logger_fatal("Invalid bootloader, please boot with multiboot2 specification");
        PANIC("Invalid bootloader, please try with valid multiboot2 loader.");
    } else {
        logger_info("Found valid bootloader with name \"%s\"", multiboot.bootloader());
    }
    
    auto* bootdata = multiboot.get_bootdata();

    segmentation_initialize();
    interrupts_initialize();

    /*auto cga_term = make<CGATerminal>(reinterpret_cast<void*>(0xB8000));
    //cga_term->disable_cursor();
    plugs::out_stream = cga_term.give_ref();*/

    acpi::initialize(bootdata);
    smbios::initialize({0xF0000, 0xFFFF});

    system_main(bootdata);
}

}