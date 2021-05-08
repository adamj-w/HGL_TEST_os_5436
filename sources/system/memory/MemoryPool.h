#pragma once

#include <system/memory/MemoryRegion.h>

#include <libruntime/Vector.h>

namespace hegel::memory {

class MemoryPool
{
private:
    size_t _quantity;
    Vector<MemoryRegion> _regions;

public:
    MemoryPool() {}
    ~MemoryPool() {}

    size_t quantity() const { return _quantity; }

    MemoryRegion take(size_t page_count);

    void take(MemoryRegion region);
    void put(MemoryRegion region);
};

}