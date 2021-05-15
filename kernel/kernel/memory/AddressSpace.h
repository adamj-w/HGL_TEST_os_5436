#pragma once

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>

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