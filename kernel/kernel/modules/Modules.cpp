#include "Modules.h"

#include <libsystem/Logger.h>

#include <string.h>

namespace modules {

typedef void (*ModuleHandlerFunc)(const boot::Module& mod);

struct ModuleHandler {
    const char* name;
    ModuleHandlerFunc handler;
};

static ModuleHandler _handlers[] = 
{
    {.name="ramdisk", .handler=ramdisk_init},
    {0, 0}
};

static void module_handle(const boot::Module& mod)
{
    ModuleHandler& handler = _handlers[0];
    while(handler.name != 0 && handler.handler != 0)
    {
        if(strcmp(handler.name, mod.command_line) == 0) {
            handler.handler(mod);
            return;
        }
    }

    logger_warn("Unknown module \"%s\"", mod.command_line);
}

void modules_initialize(const boot::Bootdata* bootdata)
{
    logger_info("Loading modules...");
    for(size_t i = 0; i < bootdata->modules_size; ++i) {
        auto& mod = bootdata->modules[i];

        logger_info("  Module %u: %#010X-%#010X: %s",
            i, mod.range.base(), mod.range.base() + mod.range.size() - 1,
            mod.command_line);

        module_handle(mod);
    }

    logger_info("%u modules loaded", bootdata->modules_size);
}

}