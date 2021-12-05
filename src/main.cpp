#include <raylib.h>

#include "logging.hpp"
#include "version.hpp"

using namespace civitas;

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

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
