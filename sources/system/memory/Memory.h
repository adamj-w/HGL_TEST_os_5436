#pragma once

#include <libruntime/RefPtr.h>
#include <libruntime/Types.h>

#include "system/memory/MemoryRegion.h"

namespace hegel::memory {

MemoryRegion alloc_region(size_t page_count);

void free_region(MemoryRegion region);

bool is_bootstrapped();

}