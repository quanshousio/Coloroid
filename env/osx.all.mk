CC := clang++
CFLAGS := $(CFLAGS:-s=)

LIB_DIRS := \
	/usr/local/lib

INCLUDE_DIRS := \
	/usr/local/include

BUILD_FLAGS :=

MACOS_FRAMEWORK_PATHS := \
	/Library/Frameworks

# Name, no extension (eg. CoreFoundation, ogg)
MACOS_FRAMEWORKS := \
	CoreFoundation \
	sfml-graphics \
	sfml-system \
 	sfml-window \
	freetype

PRODUCTION_MACOS_ICON := icon

PRODUCTION_DEPENDENCIES := \
	$(PRODUCTION_DEPENDENCIES)

PRODUCTION_MACOS_BUNDLE_VERSION := 1.0
PRODUCTION_MACOS_BUNDLE_DEVELOPER := quanshousio
PRODUCTION_MACOS_BUNDLE_DISPLAY_NAME := Coloroid
PRODUCTION_MACOS_BUNDLE_NAME := Coloroid
PRODUCTION_MACOS_CATEGORY := public.app-category.games
PRODUCTION_MACOS_MAKE_DMG := false
PRODUCTION_MACOS_BACKGROUND := dmg-background
PRODUCTION_MACOS_COPYRIGHT := "Copyright © 2018-2020 Quan Tran"

PRODUCTION_MACOS_FRAMEWORKS := \
	/Library/Frameworks/sfml-graphics \
	/Library/Frameworks/sfml-system \
 	/Library/Frameworks/sfml-window \
	/Library/Frameworks/freetype
