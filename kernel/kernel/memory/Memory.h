#pragma once

#include <libsystem/ErrorOr.h>

#include <kernel/bootdata/Bootdata.h>
#include <kernel/memory/MemoryRange.h>

namespace memory {

void memory_initialize(const boot::Bootdata* bootdata);

size_t get_used();
size_t get_total();

Error map(void* address_space, MemoryRange range, MemoryFlags flags);
Error map_identity(void* address_space, MemoryRange range, MemoryFlags flags);

ErrorOr<uintptr_t> alloc(void* address_space, size_t size, MemoryFlags flags);
Error alloc_identity(void* address_space, MemoryFlags flags, uintptr_t* out_vaddr);

Error free(void* address_space, MemoryRange range);

}
