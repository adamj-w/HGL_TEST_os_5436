#include "device/SerialStream.h"
#include "device/CGATerminal.h"
#include "segmentation/Segmentation.h"
#include "interrupts/Interrupts.h"

#include <kernel/System.h>
#include <kernel/bootdata/Multiboot2.h>
#include <kernel/bootdata/Bootdata.h>
#include <kernel/memory/Memory.h>
#include <kernel/scheduling/Scheduling.h>
#include <kernel/tasking/Tasking.h>
#include <kernel/tasking/Thread.h>
#include <kernel/acpi/ACPI.h>

#include <libsystem/Stdio.h>
#include <libsystem/Logger.h>

#include <arch/Arch.h>

using namespace hegel::arch;
using namespace hegel::arch::x86;

namespace hegel {

RefPtr<Stream> stdin;
RefPtr<Stream> stdout;
RefPtr<Stream> stderr;
RefPtr<Stream> stdlog;

void taskA()
{
    for(size_t i = 0; i < 32; i++) {
        stdout->write("A", 1);
        arch::halt();
    }

    tasking::Thread::exit();
}

void taskB()
{
    for(size_t i = 0; i < 32; i++) {
        stdout->write("B", 1);
        arch::halt();
    }

    tasking::Thread::exit();
}

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

    auto multiboot = boot::Multiboot(multiboot_addr, multiboot_magic);

    if(!multiboot.is_valid()) {
        logger_fatal("Invalid bootloader, please boot with multiboot2 specification");
        PANIC("Invalid bootloader, please try with valid multiboot2 loader.");
    } else {
        logger_info("Found valid bootloader with name \"{}\"", multiboot.bootloader());
    }

    //auto* bootdata = multiboot.get_bootdata();

    multiboot.with_memory_map([&](boot::MemoryMapEntry entry) -> Iteration {
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
    tasking::initialize();

    auto rsdp = multiboot.get_ACPI_rsdp();
    if(rsdp != nullptr) {
        logger_info("ACPI rsdp found!");
        acpi::initialize(rsdp);
    }

    /*auto framebuffer = multiboot.get_framebuffer();
    if(framebuffer == nullptr) {
        logger_fatal("This os requires a framebuffer");
    }
    graphics::initialize(framebuffer);*/

    auto task_a = tasking::Thread::create(tasking::kernel_process(), reinterpret_cast<tasking::ThreadEntry>(taskA));
    task_a->start();
    auto task_b = tasking::Thread::create(tasking::kernel_process(), reinterpret_cast<tasking::ThreadEntry>(taskB));
    task_b->start();

    print("\e[31mHegelOS\e[m (C) 2020 by Adam Warren ({} {})\n", __BUILD_TARGET__, __BUILD_GITREF__);
    print("Codename: \e[31mMarshmallow\e[m built on (\"{}\")\n", __BUILD_UNAME__);
    print("================================================================================\n");
    print("~ \e[94mh\e[m ");

    tasking::Thread::join(task_a);
    tasking::Thread::join(task_b);

    arch::shutdown();
    
    PANIC("Failed to shutdown!");
}

}