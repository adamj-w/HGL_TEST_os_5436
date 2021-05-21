#include "System.h"
#include "tasking/Thread.h"
#include "tasking/Tasking.h"

#include <libsystem/Logger.h>
#include <arch/Arch.h>

namespace hegel {

static uint64_t _current_tick;

void PANIC(const char* message) 
{
    logger_fatal("KERNEL PANIC: %s", message);

    //__alloc__::liballoc_dump();

    arch::stop();
}

void tick()
{
    __atomic_add_fetch(&_current_tick, 1, __ATOMIC_SEQ_CST);
}

uint64_t get_tick()
{
    return __atomic_load_n(&_current_tick, __ATOMIC_SEQ_CST);
}

void system_main(const boot::Bootdata* bootdata)
{
    __unused(bootdata);
    /*auto framebuffer = multiboot.get_framebuffer();
    if(framebuffer == nullptr) {
        logger_fatal("This os requires a framebuffer");
    }
    graphics::initialize(framebuffer);

    auto task_a = tasking::Thread::create(tasking::kernel_process(), reinterpret_cast<tasking::ThreadEntry>(taskA));
    task_a->start();
    auto task_b = tasking::Thread::create(tasking::kernel_process(), reinterpret_cast<tasking::ThreadEntry>(taskB));
    task_b->start();*/

    printf("\e[31mHegelOS\e[m (C) 2020 by Adam Warren (%s %s)\n", __BUILD_TARGET__, __BUILD_GITREF__);
    printf("Codename: \e[31mMarshmallow\e[m built on (\"%s\")\n", __BUILD_UNAME__);
    printf("================================================================================\n");
    printf("~ \e[94mh\e[m ");

    //tasking::Thread::join(task_a);
    //tasking::Thread::join(task_b);

    //arch::shutdown();

    arch::idle();
}

}