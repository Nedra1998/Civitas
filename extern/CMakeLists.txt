add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)

set(OpenGL_GL_PREFERENCE "GLVND")
add_subdirectory(raylib EXCLUDE_FROM_ALL)
