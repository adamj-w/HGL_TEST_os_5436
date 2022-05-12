#pragma once

#include <libsystem/Types.h>

namespace proc {

struct ProcessorContext 
{

};


enum ThreadState {
    EMBRYO,
    RUNNING,
    BLOCKED,
    STOPPED,
};

struct Thread
{
    tid_t id;
    void* entry_point;
    ThreadState state;

    uintptr_t kernel_stack_ptr;
    uintptr_t kernel_stack_top;
    uintptr_t user_stack;

    // TODO: Blocker

public:
    tid_t id() const { return id; }
    void* entry() const { return entry_point; }
    ThreadState state() const { return state; }

    Thread(void* entry);
};

}
