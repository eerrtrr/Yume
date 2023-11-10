#ifndef DEFINES_H
#define DEFINES_H

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define KPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__ANDROID__)
#define KPLATFORM_ANDROID 1
#elif defined(__linux__) || defined(__gnu_linux__)
#define KPLATFORM_LINUX 1
#elif defined(__unix__)
#define KPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define KPLATFORM_POSIX 1
#else
#error "Unknown platform!"
#endif
