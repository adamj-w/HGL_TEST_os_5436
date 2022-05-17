#include "kernel/proc/Thread.h"

#include <arch/Arch.h>

#include <kernel/interrupts/Interrupts.h>
#include <kernel/scheduler/Scheduler.h>
#include <kernel/memory/Memory.h>

#include <libutil/LinkedList.h>

#include <string.h>

using namespace hegel;

namespace proc {

static int _thread_ids = 0;
static LinkedList<Thread*>* _threads;

#define THREAD_STATE_STRING_ENTRY(__item) #__item,
const char* thread_state_strings[] = {
THREAD_STATE_LIST(THREAD_STATE_STRING_ENTRY)
};
#undef THREAD_STATE_STRING_ENTRY

Thread::Thread(Thread* parent, const char* name, ThreadFlags flags)
    : flags{flags}
{
    __unused(parent);

    ASSERT_INTERRUPTS_RETAINED();

    if(_threads == nullptr) {
        _threads = new LinkedList<Thread*>();
    }

    _id = _thread_ids++;
    strncpy(_name, name, PROCESS_NAME_SIZE);
    state = ThreadState::NONE;

    if(flags & THREAD_USER) {
        // TODO: address_space = hegel::arch::address_space_create();
    } else {
        address_space = hegel::arch::kernel_address_space();
    }

    // TODO: shared memory

    kernel_stack = memory::alloc(address_space, PROCESS_STACK_SIZE, MEMORY_CLEAR);
    kernel_stack_pointer = ((uintptr_t)kernel_stack + PROCESS_STACK_SIZE);

    hegel::arch::save_context(this);
    _threads->push_back(this);
}

Thread::~Thread() {
    interrupts::interrupts_retain();

    set_state(ThreadState::NONE);

    interrupts::interrupts_release();

    memory::free(address_space, memory::MemoryRange{kernel_stack, PROCESS_STACK_SIZE});

    if(address_space != hegel::arch::kernel_address_space()) {
        // TODO: hegel::arch::address_space_destroy(address_space);
    }
}

void Thread::go() {
    interrupts::InterruptsRetainer retainer;

    arch::thread_go(this);

    set_state(ThreadState::RUNNING);
}

void Thread::set_state(ThreadState newState) {
    ASSERT_INTERRUPTS_RETAINED();

    sched::scheduler_on_thread_state_change(this, state, newState);
    state = newState;

    if(state == ThreadState::CANCELED) {
        _threads->remove(this);
        // TODO: cleanup task
    }

}

void Thread::try_unblock() {
    if(blocker->can_unblock(*this)) {
        blocker->unblock(*this);
        set_state(ThreadState::RUNNING);
    } else if(blocker->has_timeout()) {
        blocker->timeout(*this);
        set_state(ThreadState::RUNNING);
    } else if(blocker->is_interrupted()) {
        set_state(ThreadState::RUNNING);
    }
}

uintptr_t Thread::kernel_stack_push(const void* value, size_t size) {
    kernel_stack_pointer -= size;
    memcpy((void*)kernel_stack_pointer, value, size);

    return kernel_stack_pointer;
}

OwnPtr<Thread> Thread::spawn(Thread* parent, const char* name, ThreadEntryPoint entry, void* arg, ThreadFlags flags) {
    ASSERT_INTERRUPTS_RETAINED();

    OwnPtr<Thread> thread = own<Thread>(parent, name, flags);
    thread->set_entry(entry);
    thread->kernel_stack_push(&arg, sizeof(arg));

    return thread;
}

}
