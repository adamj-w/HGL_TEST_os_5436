#pragma once

#include <libsystem/ErrorOr.h>

#include <kernel/bootdata/Bootdata.h>
#include <kernel/memory/MemoryRange.h>

namespace memory {

void memory_initialize(const boot::Bootdata* bootdata);

size_t get_used();
size_t get_total();

Result map(void* address_space, MemoryRange range, MemoryFlags flags);
Result map_identity(void* address_space, MemoryRange range, MemoryFlags flags);

ResultOr<uintptr_t> alloc(void* address_space, size_t size, MemoryFlags flags);
Result alloc_identity(void* address_space, MemoryFlags flags, uintptr_t* out_vaddr);

Result free(void* address_space, MemoryRange range);

}
