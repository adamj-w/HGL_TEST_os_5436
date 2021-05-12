#pragma once

#include <libruntime/Macros.h>

extern "C" void __assert_failed(const char* file, int line, const char* expr) __noreturn;
#define assert(__expr) if(!(__expr)) { __assert_failed(__FILE__, __LINE__, #__expr); }

extern "C" void __assert_not_reach_reached(const char* file, int line) __noreturn;
#define assert_not_reached() __assert_not_reach_reached(__FILE__, __LINE__);