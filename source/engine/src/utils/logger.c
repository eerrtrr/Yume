#include "../../includes/utils/logger.h"
#include <stdarg.h>
#include <string.h>

static LogLevel currentLogLevel = LOG_LEVEL_DEBUG; // Default log level

void set_log_level(LogLevel level) {
    currentLogLevel = level;
}

LogLevel get_current_log_level(void) {
    return currentLogLevel;
}

void log_message(LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (level < currentLogLevel) return;

    const char *levelStr = "";
    switch (level) {
        case LOG_LEVEL_DEBUG: levelStr = "[DEBUG]"; break;
        case LOG_LEVEL_INFO: levelStr = "[INFO]"; break;
        case LOG_LEVEL_WARN: levelStr = "[WARN]"; break;
        case LOG_LEVEL_ERROR: levelStr = "[ERROR]"; break;
        case LOG_LEVEL_FATAL: levelStr = "[FATAL]"; break;
    }

    printf("%s ", levelStr);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
