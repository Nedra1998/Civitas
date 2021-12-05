# Collect source code
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Generate additional sources
include(GenerateVersionHeader)
generate_version_header(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")

# Declare executable target
add_executable(civitas ${SOURCES})
target_compile_features(civitas PUBLIC cxx_std_17)

# Configure target properties
set_target_properties(civitas PROPERTIES INTERPROCEDURAL_OPTIMIZATION ${IPO})
if(CPPCHECK_EXECUTABLE AND NOT PLATFORM STREQUAL "Web")
  set_target_properties(civitas PROPERTIES CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
endif()
if(CLANG_TIDY_EXECUTABLE AND NOT PLATFORM STREQUAL "Web")
  set_target_properties(civitas PROPERTIES CXX_CLANG_TIDY
                                           ${CLANG_TIDY_EXECUTABLE})
endif()
if(IWYU_EXECUTABLE AND NOT PLATFORM STREQUAL "Web")
  set_target_properties(civitas PROPERTIES CXX_INCLUDE_WHAT_YOU_USE
                                           ${IWYU_EXECUTABLE})
endif()

# Set included directories and linked libraries
target_include_directories(
  civitas
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(civitas PUBLIC spdlog::spdlog fmt::fmt raylib)

# Configure dependency compile type options
target_compile_definitions(civitas PUBLIC -DSPDLOG_FMT_EXTERNAL)

# Configure raylib platform specific settings
if(APPLE)
  target_link_libraries(civitas "-framework IOKit")
  target_link_libraries(civitas "-framework Cocoa")
  target_link_libraries(civitas "-framework OpenGL")
endif()

# Additional compile time extensions
include(EnableExtraCompilerWarnings)
# enable_extra_compiler_warnings(civitas)
