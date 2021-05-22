#pragma once

#include <stdint.h>

extern "C" void paging_enable();
extern "C" void paging_disable();

extern "C" void paging_load_directory(uintptr_t directory);
extern "C" void paging_invalidate_tlb();

namespace paging {

void* kernel_address_space();

}
