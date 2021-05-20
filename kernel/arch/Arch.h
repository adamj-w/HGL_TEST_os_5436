#pragma once

#include <libsystem/Macros.h>
#include <libsystem/RefPtr.h>
#include <libsystem/Types.h>

#include <kernel/memory/MemoryRegion.h>

namespace hegel::tasking {
class Thread;
class Process;
typedef void (*ThreadEntry)();
}

namespace hegel::arch {

void stop() __noreturn;

void idle() __noreturn;

void halt();

void yield();

void shutdown();

size_t get_page_size();

memory::MemoryRegion get_kernel_region();

RefPtr<tasking::Thread> create_thread(RefPtr<tasking::Process> process, tasking::ThreadEntry entry);

}