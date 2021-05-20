#pragma once

#include <libsystem/Types.h>
#include <kernel/memory/Memory.h>

#include <libc/string.h>

namespace hegel::tasking {

class Stack
{
private:
    uintptr_t _ptr;
    memory::MemoryRegion _region;

public:
    Stack(size_t page_count) 
    {
        _region = memory::alloc_region(page_count);
        _ptr = _region.end_address();
    }

    ~Stack()
    {
        memory::free_region(_region);
    }

    template<typename T>
    uintptr_t push(T value)
    {
        return push<T>(&value);
    }

    template<typename T>
    uintptr_t push(T* value)
    {
        _ptr -= sizeof(T);
        memcpy(reinterpret_cast<void*>(_ptr), value, sizeof(T));
        return _ptr;
    }

    uintptr_t get_pointer() const { return _ptr; }
    void set_pointer(uintptr_t ptr) { _ptr = ptr; }
};

}