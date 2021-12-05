#include <raylib.h>

#if defined(PLATFORM_WEB)
#  include <emscripten/emscripten.h>
#endif

#include "logging.hpp"
#include "version.hpp"

using namespace civitas;

void UpdateDrawFrame();

int main(int argc, char *argv[]) {

  if (!logging::setup())
    return -1;

  LLINFO("Civitas v{}", version::semver);
#if defined(RAYLIB_VERSION)
  LLINFO("Raylib v{}", RAYLIB_VERSION);
#else
  LLINFO("Raylib v{}", "3.7.0");
#endif

  SetTraceLogCallback(logging::callback);

  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth  = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
  SetTargetFPS(60);            // Set our game to run at 60 frames-per-second
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }
#endif

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

void UpdateDrawFrame() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

  EndDrawing();
}
