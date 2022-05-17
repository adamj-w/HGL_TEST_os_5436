#include "system/System.h"
#include "memory/Memory.h"
#include "modules/Modules.h"
#include "scheduler/Scheduler.h"
#include "fs/fs.h"
#include "fs/path.h"
#include "interrupts/Interrupts.h"
#include "proc/Tasking.h"

#include <arch/Arch.h>

#include <stdio.h>
#include <string.h>
#include <libsystem/Logger.h>


namespace hegel {

void file_system_test(const char* filename) {
    auto fileE = fs::get_file(nullptr, filename);
    assert(fileE.succeed());
    auto* file = fileE.value();

    file->ops.open(file);

    char buffer[51];
    memset(buffer, '\0', 51);
    file->ops.read(file, 0, buffer, 50);

    logger_debug("Read test.txt with text \"%s\"", buffer);

    file->ops.close(file);
}

void threadA() {
    while(1) {
        putc('A', stdout);
        arch::yield();
    }
}

void threadB() {
    while(1) {
        putc('B', stdout);
        arch::yield();
    }
}

void system_main(const boot::Bootdata* bootdata)
{
    ::memory::memory_initialize(bootdata);
    sched::scheduler_initialize();
    proc::tasking_initialize();

    arch::temporary_graphics_init();

    printf("\e[31mHegelOS\e[m (C) 2020 by Adam Warren (%s %s)\n", __BUILD_TARGET__, __BUILD_GITREF__);
    printf("Codename: \e[31mMarshmallow\e[m built on (\"%s\")\n", __BUILD_UNAME__);
    printf("================================================================================\n");
    printf("~ \e[94mh\e[m ");

    interrupts::interrupts_initialize();
    
    fs::initialize_filesystem();

    modules::modules_initialize(bootdata);

    file_system_test("/other.bin");

    {
        interrupts::InterruptsRetainer retainer;

        auto tA = proc::Thread::spawn(nullptr, "threadA", threadA, nullptr, THREAD_NONE);
        auto tB = proc::Thread::spawn(nullptr, "threadB", threadB, nullptr, THREAD_NONE);

        tA->go();
        tB->go();
        tA.give_ref(); // orphan
        tB.give_ref();
        logger_debug("spawned two taskes");
    }
    arch::yield();

    panic("Reached end of implementation. Please wait for kernel to be completed.\n");
}

}