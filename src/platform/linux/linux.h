#ifndef UTIL_LINUX_PLATFORM_H_
#define UTIL_LINUX_PLATFORM_H_

#include "platform/base_platform.h"

namespace util {
struct LinuxPlatform : BasePlatform {
  LinuxPlatform();

  void setIcon(const sf::WindowHandle& inHandle) final;
  void toggleFullscreen(const sf::WindowHandle& inHandle, const sf::Uint32 inStyle,
                        const bool inWindowed, const sf::Vector2u& inResolution) final;
  float getScreenScalingFactor(const sf::WindowHandle& inHandle) final;
  int getRefreshRate(const sf::WindowHandle& inHandle) final;
};
}  // namespace util

#endif  // UTIL_LINUX_PLATFORM_H_
