#pragma once

#include <kernel/Forward.h>

extern "C" void fpu_initialize();

void fpu_save_context(proc::Thread* thread);
void fpu_load_context(proc::Thread* thread);
void fpu_init_context(proc::Thread* thread);
