#pragma once

#include <kernel/Forward.h>

#include <stdint.h>
#include <stddef.h>

namespace memory {

#define MEMORY_NONE (0)
#define MEMORY_USER (1 << 0)
#define MEMORY_CLEAR (1 << 1)

class MemoryRange
{
private:
    uintptr_t _base = 0;
    size_t _size = 0;

public:
    uintptr_t base() const { return _base; }
    uintptr_t end() const { return _base + _size + 1; }
    size_t size() const { return _size; }
    size_t page_count();
    bool empty() const { return size() == 0; }

    MemoryRange() {}
    MemoryRange(const uintptr_t base, const size_t size)
        : _base(base), _size(size) {}

    bool is_page_aligned();
    bool contains(uintptr_t address);

    static MemoryRange from_non_aligned_address(uintptr_t base, size_t size);
    static MemoryRange around_non_aligned_address(uintptr_t base, size_t size);
};

}
