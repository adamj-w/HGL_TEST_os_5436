#include "device/SerialStream.h"
#include "device/CGATerminal.h"
#include "boot/Multiboot.h"
#include "segmentation/Segmentation.h"
#include "interrupts/Interrupts.h"

#include <system/System.h>
#include <system/memory/Memory.h>
#include <system/scheduling/Scheduling.h>

#include <libsystem/Stdio.h>
#include <libsystem/Logger.h>
#include <libsystem/__alloc__.h>

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
    logger_info("Kernel built on \"{}\"", __BUILD_UNAME__);

    auto multiboot = x86::Multiboot(multiboot_addr, multiboot_magic);

    if(!multiboot.is_valid()) {
        logger_fatal("Invalid bootloader, please boot with multiboot2 specification");
        PANIC("Invalid bootloader, please try with valid multiboot2 loader.");
    } else {
        logger_info("Found valid bootloader with name \"{}\"", multiboot.bootloader());
    }

    multiboot.with_memory_map([&](MemoryMapEntry entry) {
        if(entry.is_avail()) {
            logger_info("Marking {} as usable memory.", entry);
            memory::free_region(entry.region());
        } else if(entry.is_bad()) {
            logger_warn("Badram at {}, skipping", entry.region());
        } else {
            logger_info("Skipping {}", entry);
        }

        return Iteration::CONTINUE;
    });
    
    if(!memory::is_bootstrapped()) {
        PANIC("Failed to bootstrap kernel memory allocation");
    }

    segmentation_initialize();
    interrupts_initialize();

    auto cga_term = make<CGATerminal>(reinterpret_cast<void*>(0xB8000));
    //cga_term->disable_cursor();
    stdout = cga_term;

    scheduling::initialize();

    print("\e[31mHegelOS\e[m (C) 2020 by Adam Warren ({} {})\n", __BUILD_TARGET__, __BUILD_GITREF__);
    print("Codename: \e[31mMarshmallow\e[m built on (\"{}\")\n", __BUILD_UNAME__);
    print("================================================================================\n");
    print("~ \e[94mh\e[m ");
}

}