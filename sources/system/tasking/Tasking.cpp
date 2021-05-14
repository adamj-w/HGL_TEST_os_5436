#include "Tasking.h"
#include "Process.h"
#include "Thread.h"

#include <arch/Arch.h>

#include <libsystem/Logger.h>

namespace hegel::tasking {

static void idle_task_code()
{
    do
    {
        arch::idle();
    } while (true);
}

static RefPtr<Process> _kernel_process;

void initialize()
{
    logger_info("Initializing tasking.");

    _kernel_process = make<Process>("Kernel");

    Thread::create(_kernel_process, nullptr)->start();

    Thread::create(_kernel_process, idle_task_code)->start();
}

RefPtr<Process> kernel_process()
{
    return _kernel_process;
}

}