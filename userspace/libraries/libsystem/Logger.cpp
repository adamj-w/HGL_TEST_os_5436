#include <libsystem/Logger.h>

void log(LogLevel level, const char* file, int line, ...)
{
    const char *level_names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    const char *level_colors[] = {"\e[34m", "\e[36m", "\e[32m", "\e[22m", "\e[31m", "\e[35m"};

    fprintf((FILE*)hegel::plugs::log_stream, "%s%s\e[0m ", level_colors[static_cast<int>(level)], level_names[static_cast<int>(level)]);
    fprintf((FILE*)hegel::plugs::log_stream, "%s:%d: \e[37;1m", file, line);
    va_list list;
    va_start(list, line);
    vfprintf((FILE*)hegel::plugs::log_stream, va_arg(list, const char*), list);
    va_end(list);
    fprintf((FILE*)hegel::plugs::log_stream, "\e[0m\n");
}
