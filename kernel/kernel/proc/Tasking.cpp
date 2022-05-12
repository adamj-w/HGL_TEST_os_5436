#include "Tasking.h"

#include <arch/Arch.h>

#include <libsystem/Logger.h>
#include <string.h>

namespace proc {

process_t* pqueue;
process_t* current_proc;

void scheduler_initialize()
{
    logger_info("Initializing scheduler");
    current_proc = pqueue = new process_t;
    pqueue->address_space = hegel::arch::kernel_address_space();
    pqueue->next = NULL;
}

void add_process(process_t* process)
{
    process_t* cur = pqueue;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = process;
}

uint32_t switch_task(registers_t* regs)
{
    memcpy(&current_proc->regs, regs, sizeof(registers_t));

    if(current_proc->next != NULL) current_proc = current_proc->next;
    else if(current_proc == pqueue) return 0;
    else current_proc = pqueue;

    memcpy(regs, &current_proc->regs, sizeof(registers_t));
    hegel::arch::address_space_switch(current_proc->address_space);
    return 0;
}

void kill_self()
{
    process_t* cur = pqueue;
    process_t* prev = NULL;
    while(cur != current_proc || cur != NULL) {
        prev = cur;
        cur = cur->next;
    }
    assert(cur != NULL);
    if(prev == NULL) {
        pqueue = cur->next;
        assert(pqueue != NULL);
    } else {
        prev->next = cur->next;
    }

    hegel::arch::yield();
}

}
