#pragma once

#include "defines.h"

// Should still be activated
#define LOG_FATAL_ENABLED 1
#define LOG_ERROR_ENABLED 1

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1

#if YRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#endif

typedef enum log_level
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4
} log_level;

char initialize_logger();
void stop_logger();

void log_output(log_level lvl, const char* msg, ...);
void log_output(log_level lvl, const char* msg, ...);

#ifndef YFATAL
#define YFATAL(msg, ...) log_output(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__);
#endif