# Project Setup
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(
  Civitas
  VERSION 0.1.0
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Build type dependent configuration setup
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE
      "Debug"
      CACHE STRING "Cmake build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

if("${CMAKE_BUILD_TYPE}" MATCHES "Deb")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/compile_commands.json")
    execute_process(
      COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_BINARY_DIR}/compile_commands.json"
        "${PROJECT_SOURCE_DIR}/compile_commands.json")
  endif()
endif()

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git/hooks/commit-msg"
     AND EXISTS "${PROJECT_SOURCE_DIR}/.github/hooks/commit-msg")
    execute_process(
      COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/.github/hooks/commit-msg"
        "${PROJECT_SOURCE_DIR}/.git/hooks/commit-msg")
  endif()
endif()

# Platform specific configuration
if(EMSCRIPTEN)
  set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Project Options
include(CMakeDependentOption)
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED LANGUAGES CXX)
cmake_dependent_option(IPO "Enable interprocedural optimization" ON
                       "IPO_SUPPORTED;CMAKE_BUILD_TYPE MATCHES Rel" OFF)

find_program(CCACHE_EXECUTABLE NAMES "ccache")
if(CCACHE_EXECUTABLE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
endif()

find_program(CPPCHECK_EXECUTABLE NAMES "cppcheck")
find_program(CLANG_TIDY_EXECUTABLE NAMES "clang-tidy")
find_program(IWYU_EXECUTABLE NAMES "include-what-you-use")
set(CMAKE_CXX_LINK_WHAT_YOU_USE ON)

if(EMSCRIPTEN)
  set(PLATFORM "Web")
else()
  set(PLATFORM
      "Desktop"
      CACHE STRING "Target platform to build for" FORCE)
  set_property(CACHE PLATFORM PROPERTY STRINGS "Desktop" "Web" "Android"
                                       "Raspberry Pi" "DRM")
endif()

# Fetch dependencies
include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2
  GIT_SHALLOW TRUE)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.0.1
  GIT_SHALLOW TRUE)

set(OpenGL_GL_PREFERENCE "GLVND")
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 4.0.0
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(spdlog fmt raylib)

# Targets
add_subdirectory(src)

# Build feature summary
include(FeatureSummary)
set_package_properties(
  Threads PROPERTIES
  URL "https://cmake.org/cmake/help/latest/module/FindThreads.html")
set_package_properties(Freetype PROPERTIES URL "https://freetype.org/")
set_package_properties(
  PkgConfig PROPERTIES
  URL "https://www.freedesktop.org/wiki/Software/pkg-config/")
set_package_properties(
  Fontconfig PROPERTIES
  URL "https://www.freedesktop.org/wiki/Software/fontconfig/")
set_package_properties(X11 PROPERTIES URL "https://www.x.org/wiki/")
set_package_properties(OpenGL PROPERTIES URL "https://www.opengl.org/")
set_package_properties(Git PROPERTIES URL "https://git-scm.com/")

add_feature_info(IPO IPO "Interprocedural Optimization")

feature_summary(INCLUDE_QUIET_PACKAGES QUIET_ON_EMPTY
                WHAT PACKAGES_FOUND ENABLED_FEATURES DISABLED_FEATURES)
