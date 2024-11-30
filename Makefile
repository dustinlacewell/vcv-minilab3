# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= /d/ext/vcv/Rack-SDK

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS += -ID:/ext/vcv/vcv-minilab3/src

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/**/*.cpp) src/BaseModule.cpp src/G8Pad.cpp src/MiniLog.cpp src/MiniLab3.cpp src/plugin.cpp

$(info SOURCES = $(SOURCES))

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

CXXFLAGS += -std=c++20