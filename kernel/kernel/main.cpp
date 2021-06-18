#include "system/System.h"
#include "memory/Memory.h"
#include "modules/Modules.h"
#include "fs/fs.h"

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
    
    fs::initialize_filesystem();

    modules::modules_initialize(bootdata);

    auto fileE = fs::get_file(nullptr, "/test.txt");
    assert(fileE.succeed());

    char buffer[31];
    fileE.value()->read(fileE.value(), 0, buffer, 30);
    buffer[30] = '\0';

    logger_info("Read test.txt with text \"%s\"", buffer);
}

}