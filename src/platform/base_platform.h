#ifndef UTIL_BASEPLATFORM_H_
#define UTIL_BASEPLATFORM_H_

namespace util {
struct BasePlatform {
  virtual ~BasePlatform() = default;
  virtual void setIcon(const sf::WindowHandle& inHandle) = 0;
  virtual void toggleFullscreen(const sf::WindowHandle& inHandle, const sf::Uint32 inStyle,
                                const bool inWindowed, const sf::Vector2u& inResolution) = 0;
  virtual int getRefreshRate(const sf::WindowHandle& inHandle) = 0;
  virtual float getScreenScalingFactor(const sf::WindowHandle& inHandle) = 0;
};
}  // namespace util

#endif  // UTIL_BASEPLATFORM_H_
