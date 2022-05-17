#include "Scheduler.h"

#include <kernel/system/System.h>
#include <kernel/interrupts/Interrupts.h>

#include <arch/Arch.h>

#include <libutil/LinkedList.h>
#include <libutil/OwnPtr.h>

using namespace hegel;
using namespace proc;

namespace sched {

static bool scheduler_context_switch = false;

static Thread* running = nullptr;
static OwnPtr<Thread> idle = nullptr;

static LinkedList<Thread*>* blocked_threads;
static LinkedList<Thread*>* running_threads;

void scheduler_initialize() {
    blocked_threads = new LinkedList<Thread*>();
    running_threads = new LinkedList<Thread*>();

    idle = Thread::spawn(nullptr, "idle", system_hang, nullptr, THREAD_NONE);
    idle->go();
    idle->set_state(ThreadState::HANG);

    auto kernel_task = Thread::spawn(nullptr, "system", nullptr, nullptr, THREAD_NONE);
    kernel_task->go();

    running = kernel_task.give_ref();

    // TODO: garbage collection thread
}

Thread* scheduler_running() {
    return running;
}

int scheduler_running_id() {
    if(running == nullptr) return -1;
    return running->id();
}

void scheduler_on_thread_state_change(Thread* thread, ThreadState oldState, ThreadState newState) {
    ASSERT_INTERRUPTS_RETAINED();

    if(oldState != newState) {
        if(oldState == ThreadState::RUNNING) {
            running_threads->remove(thread);
        }

        if(oldState == ThreadState::BLOCKED) {
            blocked_threads->remove(thread);
        }

        if(newState == ThreadState::BLOCKED) {
            blocked_threads->push_back(thread);
        }

        if(newState == ThreadState::RUNNING) {
            running_threads->push_back(thread);
        }
    }
}

bool scheduler_is_context_switching() {
    return scheduler_context_switch;
}

void scheduler_yield() {
    ASSERT_INTERRUPTS_NOT_RETAINED();
    arch::yield();
    ASSERT_INTERRUPTS_NOT_RETAINED();
}

uintptr_t schedule(uintptr_t esp) {
    scheduler_context_switch = true;

    running->kernel_stack_pointer = esp;
    arch::save_context(running);

    blocked_threads->foreach([](auto& task) {
        task->try_unblock();
        return Iteration::CONTINUE;
    });

    running = running_threads->peek_and_pushback();
    if(!running) running = idle.necked();

    arch::address_space_switch(running->address_space);
    arch::load_context(running);

    scheduler_context_switch = false;
    return running->kernel_stack_pointer;
}

}
