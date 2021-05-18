#pragma once

#include <libsystem/Stdio.h>
#include <libsystem/Format.h>

namespace hegel {

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
#define logger_trace(__args...) hegel::log(hegel::LogLevel::TRACE, __FILE__, __LINE__, __args)
#define logger_debug(__args...) hegel::log(hegel::LogLevel::DEBUG, __FILE__, __LINE__, __args)
#define logger_info(__args...) hegel::log(hegel::LogLevel::INFO, __FILE__, __LINE__, __args)
#define logger_warn(__args...) hegel::log(hegel::LogLevel::WARN, __FILE__, __LINE__, __args)
#define logger_error(__args...) hegel::log(hegel::LogLevel::ERROR, __FILE__, __LINE__, __args)
#define logger_fatal(__args...) hegel::log(hegel::LogLevel::FATAL, __FILE__, __LINE__, __args)
#else
#define logger_trace(__args...)
#define logger_debug(__args...)
#define logger_info(__args...) 
#define logger_warn(__args...) 
#define logger_error(__args...)
#define logger_fatal(__args...)
#endif

template<typename... Args>
void log(LogLevel level, const char* file, int line, Args... args)
{
    const char *level_names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    const char *level_colors[] = {"\e[34m", "\e[36m", "\e[32m", "\e[22m", "\e[31m", "\e[35m"};

    format(*stdlog, "{}{}\e[0m ", level_colors[static_cast<int>(level)], level_names[static_cast<int>(level)]);
    format(*stdlog, "{}:{}: \e[37;1m", file, line);
    format(*stdlog, args...);
    format(*stdlog, "\e[0m\n");
}

}