#pragma once

#include <libsystem/Macros.h>
#include <libsystem/Types.h>
#include <libsystem/Error.h>

#include <kernel/memory/MemoryRange.h>

#ifndef ARCH_PAGE_SIZE
#define ARCH_PAGE_SIZE (4096)
#endif

#define IS_PAGE_ALIGN(__x) (__x % ARCH_PAGE_SIZE == 0)

namespace hegel::arch {

// TODO: remove
void temporary_graphics_init();

void stop() __noreturn;

void idle() __noreturn;

void halt();

void yield();

void shutdown();

size_t get_page_size();

void* kernel_address_space();

void virtual_initialize();

void virtual_memory_enable();

bool virtual_present(void* address_space, uintptr_t virtual_address);

uintptr_t virtual_to_physical(void* address_space, uintptr_t virtual_address);

Result virtual_map(void* address_space, memory::MemoryRange physical_range, uintptr_t virtual_address, memory::MemoryFlags flags);

memory::MemoryRange virtual_alloc(void* address_space, memory::MemoryRange physical_range, memory::MemoryFlags flags);
void virtual_free(void* address_space, memory::MemoryRange physical_range);

void* address_space_create();
void address_space_destroy(void* address_space);

void address_space_switch(void* address_space);


}