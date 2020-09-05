#ifndef PLATFORM_H_
#define PLATFORM_H_

#if defined(__APPLE__)
#include "platform/macos/macos.h"
#elif defined(__linux__)
#include "platform/linux/linux.h"
#elif defined(_WIN32)
#include "platform/win32/win32.h"
#endif

namespace util {
#if defined(__APPLE__)
using Platform = MacOSPlatform;
#elif defined(__linux__)
using Platform = LinuxPlatform;
#elif defined(_WIN32)
using Platform = WindowsPlatform;
#endif
}  // namespace util

#endif  // UTIL_PLATFORM_H_
