#include "system/System.h"
#include "memory/Memory.h"

#include <arch/Arch.h>

#include <stdio.h>
#include <libsystem/Logger.h>

namespace hegel {

void system_main(const boot::Bootdata* bootdata)
{
    ::memory::memory_initialize(bootdata);

    arch::temporary_graphics_init();

    printf("\e[31mHegelOS\e[m (C) 2020 by Adam Warren (%s %s)\n", __BUILD_TARGET__, __BUILD_GITREF__);
    printf("Codename: \e[31mMarshmallow\e[m built on (\"%s\")\n", __BUILD_UNAME__);
    printf("================================================================================\n");
    printf("~ \e[94mh\e[m ");

    //tasking::Thread::join(task_a);
    //tasking::Thread::join(task_b);

    //arch::shutdown();
}

}