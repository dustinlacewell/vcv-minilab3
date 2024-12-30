# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# Set the deployment target for macOS
OSX_DEPLOYMENT_TARGET ?= 10.9

# FLAGS will be passed to both the C and C++ compiler
FLAGS += -isystem $(RACK_DIR)/include
CFLAGS +=
CXXFLAGS += -Isrc

# Add macOS deployment target flag if on macOS
ifeq ($(shell uname), Darwin)
    CFLAGS += -mmacosx-version-min=$(OSX_DEPLOYMENT_TARGET)
    CXXFLAGS += -mmacosx-version-min=$(OSX_DEPLOYMENT_TARGET)
endif

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/**/*.cpp)
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

CXXFLAGS += -std=c++20