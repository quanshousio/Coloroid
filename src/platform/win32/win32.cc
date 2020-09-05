#ifdef _WIN32
#include "platform/win32/win32.h"

#include "resource.h"

namespace util {
WindowsPlatform::WindowsPlatform() {
  // Get the icon directory
  PBYTE iconDirectory = getIconDirectory(WIN32_ICON_MAIN);

  // Get the default device info
  m_screenScalingFactor = getScreenScalingFactor(nullptr);
  m_refreshRate = getRefreshRate(nullptr);

  // Store each icon
  std::array<int, 6> icons = {16, 32, 48, 64, 128};
  for (auto& it : icons) {
    HICON icon = getIconFromIconDirectory(iconDirectory, it);
    m_hIcons.push_back(icon);
  }
}

WindowsPlatform::~WindowsPlatform() {
  for (auto& it : m_hIcons) {
    if (it) DestroyIcon(it);
  }

  m_hIcons.clear();
}

void WindowsPlatform::setIcon(const sf::WindowHandle& inHandle) {
  std::size_t indexSmallIcon =
      static_cast<std::size_t>(std::min(std::max(std::ceil(m_screenScalingFactor - 1.0f), 0.0f),
                                        static_cast<float>(m_hIcons.size()) - 1.0f));
  std::size_t indexBigIcon = static_cast<std::size_t>(
      std::min(std::max(std::ceil(m_screenScalingFactor - 1.0f), 0.0f) + 1.0f,
               static_cast<float>(m_hIcons.size()) - 1.0f));

  if (m_hIcons[indexBigIcon])
    SendMessage(inHandle, WM_SETICON, ICON_BIG, (LPARAM)m_hIcons[indexBigIcon]);

  if (m_hIcons[indexSmallIcon])
    SendMessage(inHandle, WM_SETICON, ICON_SMALL, (LPARAM)m_hIcons[indexSmallIcon]);
}

void WindowsPlatform::toggleFullscreen(const sf::WindowHandle& inHandle, const sf::Uint32 inStyle,
                                       const bool inWindowed, const sf::Vector2u& inResolution) {
  DWORD win32Style = sfmlWindowStyleToWin32WindowStyle(inStyle);
  UINT flags = SWP_DRAWFRAME | SWP_FRAMECHANGED;

  if (inWindowed) {
    // window (centered on the focused screen)
    HDC screenDC = GetDC(inHandle);
    int screenWidth = GetDeviceCaps(screenDC, HORZRES);
    int screenHeight = GetDeviceCaps(screenDC, VERTRES);
    ReleaseDC(inHandle, screenDC);

    int width = static_cast<int>(inResolution.x);
    int height = static_cast<int>(inResolution.y);
    int left = (screenWidth - width) / 2;
    int top = (screenHeight - height) / 2;
    RECT rectangle = {0, 0, width, height};
    AdjustWindowRect(&rectangle, win32Style, false);
    width = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;

    SetWindowLongPtr(inHandle, GWL_STYLE, win32Style);
    SetWindowLongPtr(inHandle, GWL_EXSTYLE, 0);
    SetWindowPos(inHandle, nullptr, left, top, width, height, flags);
  } else {
    // fullscreen
    int width = static_cast<int>(inResolution.x);
    int height = static_cast<int>(inResolution.y);

    // first time prevents the border from showing in the corner
    SetWindowPos(inHandle, HWND_TOP, 0, 0, width, height, flags);
    SetWindowLongPtr(inHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
    SetWindowLongPtr(inHandle, GWL_STYLE, win32Style);

    // second time cleans up the rect after the border has been removed
    SetWindowPos(inHandle, HWND_TOP, 0, 0, width, height, flags);

    // note: double SetWindowPos call isn't very effective on slower machines anyway
  }
  ShowWindow(inHandle, SW_SHOW);
}

float WindowsPlatform::getScreenScalingFactor(const sf::WindowHandle& inHandle) {
  UNUSED(inHandle);

  if (m_screenScalingFactor != 0.0f) return m_screenScalingFactor;

  HDC screenDC = GetDC(nullptr);
  int logicalScreenHeight = GetDeviceCaps(screenDC, VERTRES);
  int physicalScreenHeight = GetDeviceCaps(screenDC, DESKTOPVERTRES);
  m_screenScalingFactor =
      static_cast<float>(physicalScreenHeight) / static_cast<float>(logicalScreenHeight);
  ReleaseDC(nullptr, screenDC);

  return m_screenScalingFactor;
}

int WindowsPlatform::getRefreshRate(const sf::WindowHandle& inHandle) {
  UNUSED(inHandle);

  if (m_refreshRate != 0) return m_refreshRate;

  HDC screenDC = GetDC(nullptr);
  m_refreshRate = GetDeviceCaps(screenDC, VREFRESH);
  ReleaseDC(nullptr, screenDC);

  return m_refreshRate;
}

PBYTE WindowsPlatform::getIconDirectory(const int inResourceId) {
  HMODULE hModule = GetModuleHandle(nullptr);
  HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(inResourceId), RT_GROUP_ICON);

  HGLOBAL hData = LoadResource(hModule, hResource);
  PBYTE data = (PBYTE)LockResource(hData);

  return data;
}

HICON WindowsPlatform::getIconFromIconDirectory(PBYTE inIconDirectory, const uint inSize) {
  HMODULE hModule = GetModuleHandle(nullptr);
  int resourceId =
      LookupIconIdFromDirectoryEx(inIconDirectory, TRUE, inSize, inSize, LR_DEFAULTCOLOR);
  HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), RT_ICON);

  HGLOBAL hData = LoadResource(hModule, hResource);
  PBYTE data = (PBYTE)LockResource(hData);
  DWORD sizeofData = SizeofResource(hModule, hResource);

  HICON icon =
      CreateIconFromResourceEx(data, sizeofData, TRUE, 0x00030000, inSize, inSize, LR_DEFAULTCOLOR);
  return icon;
}

DWORD WindowsPlatform::sfmlWindowStyleToWin32WindowStyle(const sf::Uint32 inStyle) {
  DWORD style = 0;
  if (inStyle == sf::Style::None || inStyle == sf::Style::Fullscreen) {
    style = WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  } else {
    style = WS_VISIBLE;
    if (inStyle & sf::Style::Titlebar) style |= WS_CAPTION | WS_MINIMIZEBOX;
    if (inStyle & sf::Style::Resize) style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
    if (inStyle & sf::Style::Close) style |= WS_SYSMENU;
  }

  return style;
}
}  // namespace util

#endif  // _WIN32
