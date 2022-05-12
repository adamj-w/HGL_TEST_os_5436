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

#include <libsystem/Macros.h>

char fpu_initial_context[512] __aligned(16);

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
