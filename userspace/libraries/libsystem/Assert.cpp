#include <libsystem/Assert.h>
#include <libsystem/Logger.h>
#include <libsystem/__plugs__.h>

extern "C" void __assert_failed(const char* file, int line, const char* expr) {
    logger_fatal("Assertion failed \"%s\" at %s:%d", expr, file, line);

    hegel::plugs::assert_failed();
}

extern "C" void __assert_not_reach_reached(const char* file, int line) {
    logger_fatal("Expression reached %s:%d", file, line);

    hegel::plugs::assert_not_reached_reached();
}