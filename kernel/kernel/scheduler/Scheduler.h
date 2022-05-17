#pragma once

#include <kernel/proc/Thread.h>

namespace sched {

void scheduler_initialize();

proc::Thread* scheduler_running();

int scheduler_running_id();

void scheduler_on_thread_state_change(proc::Thread* thread, proc::ThreadState oldState, proc::ThreadState newState);

void scheduler_yield();

uintptr_t schedule(uintptr_t current_esp);

}
