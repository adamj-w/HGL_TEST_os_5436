#include "Memory.h"
#include "Physical.h"

#include <libsystem/Logger.h>
#include <string.h>

#include <arch/Arch.h>

using namespace boot;
using namespace hegel::arch;

namespace memory {

static bool _memory_initialized = false;
extern "C" int __start;
extern "C" int __end;

static MemoryRange kernel_memory_range()
{
    return MemoryRange::around_non_aligned_address((uintptr_t)&__start, (size_t)&__end - (size_t)&__start);
}

void memory_initialize(const Bootdata* bootdata)
{
    logger_info("Initializing memory");

    for(size_t i = 0; i < 1024 * 1024 / 8; i++) {
        MEMORY[i] = 0xff;
    }

    for(size_t i = 0; i < bootdata->memory_map_size; i++) {
        const MemoryMapEntry* entry = &bootdata->memory_map[i];

        if(entry->type == MEMORY_MAP_ENTRY_AVAILABLE) {
            physical_set_free(entry->range);
        }
    }

    virtual_initialize();

    USED_MEMORY = 0;
    TOTAL_MEMORY = bootdata->memory_usable;

    logger_info("Mapping kernel...");
    map_identity(kernel_address_space(), kernel_memory_range(), MEMORY_NONE);

    logger_info("Mapping modules...");
    for(size_t i = 0; i < bootdata->modules_size; i++) {
        map_identity(kernel_address_space(), bootdata->modules[i].range, MEMORY_NONE);
    }

    MemoryRange page_zero{0, ARCH_PAGE_SIZE};
    virtual_free(kernel_address_space(), page_zero);
    physical_set_used(page_zero);

    address_space_switch(kernel_address_space());

    virtual_memory_enable();

    logger_info("%uKiB of memory detected", TOTAL_MEMORY / 1024);
    logger_info("%uKiB of memory is used by the kernel", USED_MEMORY / 1024);

    _memory_initialized = true;
}

size_t get_used()
{
    return USED_MEMORY;
}

size_t get_total()
{
    return TOTAL_MEMORY;
}

Result map(void* address_space, MemoryRange virtual_range, MemoryFlags flags)
{
    assert(virtual_range.is_page_aligned());

    for(size_t i = 0; i < virtual_range.size() / ARCH_PAGE_SIZE; i++) {
        uintptr_t vaddr = virtual_range.base() + i * ARCH_PAGE_SIZE;

        if(!virtual_present(address_space, vaddr)) {
            auto physical_range = physical_alloc(ARCH_PAGE_SIZE);
            Result vmap = virtual_map(address_space, physical_range, vaddr, flags);

            if(vmap != Result::SUCCEED) {
                return vmap;
            }
        }
    }

    if(flags & MEMORY_CLEAR) {
        memset((void*)virtual_range.base(), 0, virtual_range.size());
    }

    return Result::SUCCEED;
}

Result map_identity(void* address_space, MemoryRange physical_range, MemoryFlags flags)
{
    assert(physical_range.is_page_aligned());

    physical_set_used(physical_range);
    assert(virtual_map(address_space, physical_range, physical_range.base(), flags) == Result::SUCCEED);

    if(flags & MEMORY_CLEAR) {
        memset((void*)physical_range.base(), 0, physical_range.size());
    }

    return Result::SUCCEED;
}

ResultOr<uintptr_t> alloc(void* address_space, size_t size, MemoryFlags flags)
{
    assert(IS_PAGE_ALIGN(size));

    if(!size) {
        logger_warn("Allocation with size=0!\n");
        return ResultOr<uintptr_t>(Result::SUCCEED, 0);
    }

    auto physical_range = physical_alloc(size);

    if(physical_range.empty()) {
        logger_error("Failed to allocate memory: Not enough physical memory!\n");
        return Result::OUT_OF_MEMORY;
    }

    uintptr_t vaddr = virtual_alloc(address_space, physical_range, flags).base();

    if(!vaddr) {
        physical_free(physical_range);

        logger_error("Failed to allocate memoty: Not enough virtual memory!\n");
        return Result::OUT_OF_MEMORY;
    }

    if(flags & MEMORY_CLEAR) {
        memset((void*)vaddr, 0, size);
    }

    return ResultOr<uintptr_t>(vaddr);
}

Result alloc_identity(void* address_space, MemoryFlags flags, uintptr_t* out_vaddr)
{
    for(size_t i = 1; i < 256 * 1024; i++) {
        MemoryRange identity_range{i * ARCH_PAGE_SIZE, ARCH_PAGE_SIZE};

        if(!virtual_present(address_space, identity_range.base()) &&
            !physical_is_used(identity_range))
        {
            physical_set_used(identity_range);
            assert(virtual_map(address_space, identity_range, identity_range.base(), flags) == Result::SUCCEED);

            if(flags & MEMORY_CLEAR) {
                memset((void*)identity_range.base(), 0, ARCH_PAGE_SIZE);
            }

            *out_vaddr = identity_range.base();

            return Result::SUCCEED;
        }
    }

    logger_error("Failed to allocate identity mapped page!\n");

    *out_vaddr = 0;
    return Result::OUT_OF_MEMORY;
}

Result free(void* address_space, MemoryRange vrange)
{
    assert(vrange.is_page_aligned());
    assert(vrange.size() > 0);

    for(size_t i = 0; i < vrange.size() / ARCH_PAGE_SIZE; i++) {
        uintptr_t vaddr = vrange.base() + i * ARCH_PAGE_SIZE;

        if(virtual_present(address_space, vaddr)) {
            MemoryRange page_physical_range{virtual_to_physical(address_space, vaddr), ARCH_PAGE_SIZE};
            MemoryRange page_virtual_range{vaddr, ARCH_PAGE_SIZE};

            physical_free(page_physical_range);
            virtual_free(address_space, page_virtual_range);
        }
    }

    return Result::SUCCEED;
}

}
