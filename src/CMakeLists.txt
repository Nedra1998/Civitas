file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include(GenerateVersionHeader)
generate_version_header(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp")

add_executable(civitas ${SOURCES})
target_compile_features(civitas PUBLIC cxx_std_17)

set_target_properties(civitas PROPERTIES INTERPROCEDURAL_OPTIMIZATION
                                         ${CIVITAS_IPO})
if(CPPCHECK_EXECUTABLE)
  set_target_properties(civitas PROPERTIES CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
endif()
if(CLANG_TIDY_EXECUTABLE)
  set_target_properties(civitas PROPERTIES CXX_CLANG_TIDY
                                           ${CLANG_TIDY_EXECUTABLE})
endif()
if(IWYU_EXECUTABLE)
  set_target_properties(civitas PROPERTIES CXX_INCLUDE_WHAT_YOU_USE
                                           ${IWYU_EXECUTABLE})
endif()

target_compile_definitions(civitas PUBLIC -DSPDLOG_FMT_EXTERNAL)

target_include_directories(
  civitas
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(civitas PUBLIC spdlog::spdlog fmg::fmt cereal::cereal
                                     raylib)

include(EnableExtraCompilerWarnings)
enable_extra_compiler_warnings(civitas)
