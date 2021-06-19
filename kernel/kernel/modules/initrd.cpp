#include "Modules.h"
#include "../memory/Memory.h"
#include "../fs/fs.h"

#include <libsystem/Macros.h>
#include <libsystem/Logger.h>
#include <libsystem/Tar.h>
#include <string.h>

#include <arch/Arch.h>

#define min(x, y) (x < y ? x : y)

using namespace hegel::arch;

namespace modules {

static Result initrd_open(fs::file*) { return Result::SUCCEED; }
static void initrd_close(fs::file*) { }

static void initrd_read(fs::file* file, uint32_t off, void* buffer, size_t n)
{
    TarBlock* block = (TarBlock*)file->data;
    uintptr_t data = (uintptr_t)block->data + off;
    size_t end = min(block->size - off, off + n);

    memcpy(buffer, (void*)data, end);
}

void ramdisk_init(const boot::Module& mod)
{
    fs::file_ops file_operations = {};
    file_operations.open = initrd_open;
    file_operations.close = initrd_close;
    file_operations.read = initrd_read;

    ResultOr<TarBlock*> blockE = Result::END_OF_STREAM;
    for(
        size_t i = 0; 
        (blockE = TarBlock::read((void*)mod.range.base(), i)).succeed(); 
        i++)
    {
        auto* block = blockE.value();
        if(block->name[strlen(block->name) - 1] == '/') {
            fs::directory_create(nullptr, block->name, 0);
        } else {
            //logger_debug("Found file with name \"%s\" at %#010X", block->name, block->data);
            fs::file_create(nullptr, block->name, (void*)block, 0, file_operations);
        }
    }

    //memory::free(kernel_address_space(), mod.range);
}

}