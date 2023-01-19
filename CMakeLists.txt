cmake_minimum_required(VERSION 3.16)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")

set(CMAKE_CXX_STANDARD 23)

project(Minilab3)

set(PLUGIN_NAME ${PROJECT_NAME})
set(ADDITIONAL_PLUGIN_DISTRIBUTABLES README.md)
include(RackSDK.cmake)

include_directories(include)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
target_sources(${RACK_PLUGIN_LIB} PRIVATE ${SOURCE_FILES})

