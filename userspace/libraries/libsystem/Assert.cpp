#include <libsystem/Assert.h>
#include <libsystem/Logger.h>
#include <libsystem/__plugs__.h>

extern "C" void __assert_failed(const char* file, int line, const char* expr) {
    logger_fatal("Assertion failed \"{}\" at {}:{d}", expr, file, line);

    hegel::plugs::assert_failed();
}

extern "C" void __assert_not_reach_reached(const char* file, int line) {
    logger_fatal("Expression reached {}:{d}", file, line);

    hegel::plugs::assert_not_reached_reached();
}