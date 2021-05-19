#pragma once

enum class LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

#ifdef __KERNEL__
#define logger_trace(__args...) ::log(LogLevel::TRACE, __FILE__, __LINE__, __args)
#define logger_debug(__args...) ::log(LogLevel::DEBUG, __FILE__, __LINE__, __args)
#define logger_info(__args...) ::log(LogLevel::INFO, __FILE__, __LINE__, __args)
#define logger_warn(__args...) ::log(LogLevel::WARN, __FILE__, __LINE__, __args)
#define logger_error(__args...) ::log(LogLevel::ERROR, __FILE__, __LINE__, __args)
#define logger_fatal(__args...) ::log(LogLevel::FATAL, __FILE__, __LINE__, __args)
#else
#define logger_trace(__args...)
#define logger_debug(__args...)
#define logger_info(__args...) 
#define logger_warn(__args...) 
#define logger_error(__args...)
#define logger_fatal(__args...)
#endif

#include <libsystem/__plugs__.h>
#include <stdio.h>
#include <stdarg.h>

void log(LogLevel level, const char* file, int line, ...);
