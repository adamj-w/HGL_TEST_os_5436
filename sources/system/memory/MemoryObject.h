#pragma once

#include <libruntime/RefCounted.h>

#include <system/memory/MemoryRegion.h>

namespace hegel::memory {

class MemoryObject : public RefCounted<MemoryObject>
{
private:
    int _id;
    MemoryRegion _region;

public:
    int id() const { return _id; }
    MemoryRegion region() const { return _region; }

    MemoryObject(MemoryRegion region);
    ~MemoryObject();
};

}