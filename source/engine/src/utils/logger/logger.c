#include "logger.h"
#include <stdarg.h>
#include <string.h>
#include "../../platform/platform.h"

static LogLevel currentLogLevel = LOG_LEVEL_DEBUG; // Default log level

void set_log_level(LogLevel level) {
    currentLogLevel = level;
}

LogLevel get_current_log_level(void) {
    return currentLogLevel;
}

void log_message(LogLevel level, const char *file, int line, const char *fmt, ...) {
    UNUSED(file);
    UNUSED(line);
    
    if (level < currentLogLevel) return;

    const char *levelStr = "";
    switch (level) {
        case LOG_LEVEL_DEBUG: levelStr = "[DEBUG]"; break;
        case LOG_LEVEL_INFO: levelStr = "[INFO]"; break;
        case LOG_LEVEL_WARN: levelStr = "[WARN]"; break;
        case LOG_LEVEL_ERROR: levelStr = "[ERROR]"; break;
        case LOG_LEVEL_FATAL: levelStr = "[FATAL]"; break;
    }

    const int32_t msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));

    printf("%s ", levelStr);
    va_list args;
    va_start(args, fmt);
    vsnprintf(out_message, msg_length, fmt, args);
    va_end(args);

    char out_message2[msg_length];
    sprintf(out_message2, "%s%s\n", levelStr, out_message);

    platform_console_write(out_message2, level);

    printf("\n");
}