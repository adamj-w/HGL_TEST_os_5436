#pragma once

#include <libsystem/LinkedList.h>
#include <libsystem/RefPtr.h>

#include <kernel/memory/MemoryObject.h>

namespace hegel::memory {

class AddressSpace
{
private:   
    LinkedList<RefPtr<MemoryObject>> _objects;

public:
    AddressSpace() {}
    ~AddressSpace() {}
};

}