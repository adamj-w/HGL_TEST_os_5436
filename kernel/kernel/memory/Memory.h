#pragma once

#include <libsystem/RefPtr.h>
#include <libsystem/Types.h>

#include "MemoryRegion.h"

namespace hegel::memory {

MemoryRegion alloc_region(size_t page_count);

void free_region(MemoryRegion region);

bool is_bootstrapped();

}