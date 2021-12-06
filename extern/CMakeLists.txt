add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)

set(OpenGL_GL_PREFERENCE "GLVND")
add_subdirectory(raylib EXCLUDE_FROM_ALL)

add_library(raygui "${CMAKE_CURRENT_SOURCE_DIR}/raygui.c")
target_link_libraries(raygui PUBLIC raylib)
target_include_directories(raygui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/raygui/src")
