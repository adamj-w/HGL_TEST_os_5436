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

#include <libsystem/__plugs__.h>
#include <libsystem/Logger.h>

#include <arch/Arch.h>

using namespace hegel::arch;
using namespace hegel::arch::x86;

namespace hegel {

void taskA()
{
    for(size_t i = 0; i < 32; i++) {
        plugs::out_stream->write("A", 1);
        arch::halt();
    }

    tasking::Thread::exit();
}

void taskB()
{
    for(size_t i = 0; i < 32; i++) {
        plugs::out_stream->write("B", 1);
        arch::halt();
    }

    tasking::Thread::exit();
}

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

    //auto* bootdata = multiboot.get_bootdata();

    multiboot.with_memory_map([&](boot::MemoryMapEntry entry) -> Iteration {
        if(entry.is_avail()) {
            logger_info("Marking %#010X as usable memory.", (uint32_t)&entry);
            memory::free_region(entry.region());
        } else if(entry.is_bad()) {
            // TODO: logger_warn("Badram at {}, skipping", (uint32_t)entry.region());
        } else {
            logger_info("Skipping %#010X", (uint32_t)&entry);
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
    plugs::out_stream = cga_term.give_ref();

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

    printf("\e[31mHegelOS\e[m (C) 2020 by Adam Warren (%s %s)\n", __BUILD_TARGET__, __BUILD_GITREF__);
    printf("Codename: \e[31mMarshmallow\e[m built on (\"%s\")\n", __BUILD_UNAME__);
    printf("================================================================================\n");
    printf("~ \e[94mh\e[m ");

    tasking::Thread::join(task_a);
    tasking::Thread::join(task_b);

    arch::shutdown();
    
    PANIC("Failed to shutdown!");
}

}