#pragma once

#include <libsystem/ErrorOr.h>
#include <libsystem/Types.h>
#include <libsystem/Macros.h>

#include <libsystem/Stream.h>

extern "C" void __plug_init_libsystem(hegel::Stream* stream);
extern "C" void __plug_fini_libsystem();

namespace hegel::plugs {

extern Stream* in_stream;
extern Stream* out_stream;
extern Stream* err_stream;
extern Stream* log_stream;

bool memory_is_locked();
void memory_lock();
void memory_unlock();
size_t get_page_size();
ErrorOr<uintptr_t> memory_alloc(size_t page_count);
void memory_free(uintptr_t addr, size_t page_count);

void assert_failed() __noreturn;
void assert_not_reached_reached() __noreturn;

}