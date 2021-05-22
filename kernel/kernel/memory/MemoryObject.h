#pragma once

#include <kernel/memory/MemoryRange.h>

namespace memory {

struct MemoryObject
{
    int id;
    MemoryRange _range;
    
    int refcount;

    MemoryRange range() { return _range; }
};

void object_initialize();

MemoryObject* object_create(size_t size);
void object_destroy(MemoryObject* object);

MemoryObject* object_ref(MemoryObject* object);
void object_deref(MemoryObject* object);

MemoryObject* object_by_id(int id);

}