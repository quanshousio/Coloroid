#ifndef UTIL_WIN32_PLATFORM_H_
#define UTIL_WIN32_PLATFORM_H_

#include "platform/base_platform.h"

// TODO: WM_DISPLAYCHANGE event handling (multi-monitor support)

namespace util {
struct WindowsPlatform : BasePlatform {
  WindowsPlatform();
  ~WindowsPlatform();

  void setIcon(const sf::WindowHandle& inHandle) final;
  void toggleFullscreen(const sf::WindowHandle& inHandle, const sf::Uint32 inStyle,
                        const bool inWindowed, const sf::Vector2u& inResolution) final;
  float getScreenScalingFactor(const sf::WindowHandle& inHandle) final;
  int getRefreshRate(const sf::WindowHandle& inHandle) final;

 private:
  PBYTE getIconDirectory(const int inResourceId);
  HICON getIconFromIconDirectory(PBYTE inIconDirectory, const uint inSize);
  DWORD sfmlWindowStyleToWin32WindowStyle(const sf::Uint32 inStyle);

  std::vector<HICON> m_hIcons;

  float m_screenScalingFactor = 0.0f;
  int m_refreshRate = 0;
};
}  // namespace util

#endif  // UTIL_WIN32_PLATFORM_HPP
