#include "Modules.h"
#include "../memory/Memory.h"
#include "../fs/fs.h"

#include <libsystem/Macros.h>
#include <libsystem/Logger.h>
#include <libsystem/Tar.h>
#include <string.h>

#include <arch/Arch.h>

using namespace hegel::arch;

namespace modules {

void ramdisk_init(const boot::Module& mod)
{
    ResultOr<TarBlock*> blockE = Result::END_OF_STREAM;
    for(
        size_t i = 0; 
        (blockE = TarBlock::read((void*)mod.range.base(), i)).succeed(); 
        i++)
    {
        auto* block = blockE.value();
        if(block->name[strlen(block->name) - 1] == '/') {
            //fs::directory_create(nullptr, block->name, 0);
        } else {
            logger_debug("Found file with name \"%s\" at %#010X", block->name, block->data);
            //fs::file_create(nullptr, block->name, 0);
        }
    }

    //memory::free(kernel_address_space(), mod.range);
}

}