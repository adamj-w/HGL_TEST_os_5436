#pragma once

#include <libruntime/ErrorOr.h>
#include <libruntime/Types.h>
#include <libruntime/Macros.h>

extern "C" void __plug_init_libsystem();

namespace hegel::plugs {

bool memory_is_locked();
void memory_lock();
void memory_unlock();
size_t get_page_size();
ErrorOr<uintptr_t> memory_alloc(size_t page_count);
void memory_free(uintptr_t addr, size_t page_count);

void assert_failed() __noreturn;
void assert_not_reached_reached() __noreturn;

}