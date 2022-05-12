#pragma once

#include <libsystem/Types.h>
#include <libsystem/Macros.h>

namespace proc {

typedef struct __packed InterruptStackFrame
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t intno, err;
    uint32_t eip, cs, eflags;
} registers_t;

typedef struct process_struct
{
    registers_t regs;
    void* address_space;
    struct process_struct* next;
} process_t;

void scheduler_initialize();

void add_process(process_t* process);

uint32_t switch_task(registers_t* regs);

void kill_self();

}
