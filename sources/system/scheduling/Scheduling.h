#pragma once

#include <libruntime/RefPtr.h>
#include <system/tasking/Thread.h>

namespace hegel::scheduling {

void initialize();

void update_thread_state(RefPtr<tasking::Thread> thread, tasking::ThreadState new_state);

// Called by interrupt_common
uintptr_t schedule(uintptr_t stack_pointer);

RefPtr<tasking::Thread> running_thread();
RefPtr<tasking::Process> running_process();

}