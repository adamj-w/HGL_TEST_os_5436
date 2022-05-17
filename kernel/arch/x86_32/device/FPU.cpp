// section .text
// global fpu_initialize:function (fpu_initialize.end - fpu_initialize)
// fpu_initialize:
//     push ebp
//     mov ebp, esp

//     mov edx, cr0
//     and edx, (-1) - ((1 <<))

//     mov esp, ebp
//     pop ebp
//     ret

// .end:

#include <stddef.h>
#include <string.h>

#include <libsystem/Macros.h>
#include <kernel/proc/Thread.h>

using namespace proc;

char fpu_initial_context[512] __aligned(16);
char fpu_registers[512] __aligned(16);

extern "C" void fpu_initialize() {
    asm volatile("clts");
    size_t t;
    asm volatile("mov %%cr0, %0" : "=r"(t));
    t &= ~(1 << 2);
    t |= (1 << 1);
    t |= (1 << 5);
    asm volatile("mov %0, %%cr0" :: "r"(t));

    asm volatile("mov %%cr4, %0" : "=r"(t));

    t |= 3 << 9;
    asm volatile("mov %0, %%cr4" :: "r"(t));

    asm volatile("fninit");
    asm volatile("fxsave (%0)" :: "r"(fpu_initial_context));
}

void fpu_init_context(Thread* thread) {
    memcpy(thread->fpu_registers, fpu_initial_context, 512);
}

void fpu_save_context(Thread* thread) {
    asm volatile("fxsave (%0)" ::"r"(fpu_registers));
    memcpy(thread->fpu_registers, fpu_registers, 512);
}

void fpu_load_context(Thread* thread) {
    memcpy(fpu_registers, thread->fpu_registers, 512);
    asm volatile("fxrstor (%0)" ::"r"(fpu_registers));
}
