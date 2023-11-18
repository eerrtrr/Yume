#pragma once

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#elif defined(__linux__) || defined(__gnu_linux__)
#define PLATFORM_LINUX 1
#elif defined(__unix__)
#define PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define PLATFORM_POSIX 1
#else
#error "Unknown platform!"
#endif


//Mark unused variables
#define UNUSED(x) (void)(x)