#include <raygui.h>
#include <raylib.h>

#if defined(PLATFORM_WEB)
#  include <emscripten/emscripten.h>
#endif

#include "logging.hpp"
#include "version.hpp"

using namespace civitas;

static int screenWidth  = 1024;
static int screenHeight = 576;

void UpdateDrawFrame();

struct RayguiEditable {
  int value = 0;
  bool edit = false;
};

class MainLoop {
public:
  Vector2 scroll = {0, 0};

  RayguiEditable mode   = {0, false};
  RayguiEditable width  = {128, false};
  RayguiEditable height = {128, false};

  float speed     = 256.0f;
  Camera2D camera = {0};

  Texture2D bg_tex;

  MainLoop() {
    Image bg_img =
        GenImageChecked(screenWidth * 2 + 128, screenHeight * 2 + 128, 32, 32,
                        GetColor(0x2e222fff), GetColor(0x3e3546ff));
    bg_tex = LoadTextureFromImage(bg_img);
    UnloadImage(bg_img);

    camera.target   = (Vector2){0, 0};
    camera.offset   = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom     = 2.5f;
  }

  ~MainLoop() { UnloadTexture(bg_tex); }

  void run() {
    float deltaTime = GetFrameTime();

    if (IsWindowResized()) {
      screenWidth  = GetScreenWidth();
      screenHeight = GetScreenHeight();

      UnloadTexture(bg_tex);
      Image bg_img =
          GenImageChecked(screenWidth * 2 + 128, screenHeight * 2 + 128, 32, 32,
                          GetColor(0x2e222fff), GetColor(0x3e3546ff));
      bg_tex = LoadTextureFromImage(bg_img);
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      camera.target.x -= speed * deltaTime;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      camera.target.x += speed * deltaTime;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      camera.target.y -= speed * deltaTime;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      camera.target.y += speed * deltaTime;
    if (IsKeyDown(KEY_E))
      camera.zoom = std::min(5.0f, camera.zoom + (deltaTime));
    if (IsKeyDown(KEY_Q))
      camera.zoom = std::max(0.5f, camera.zoom - (deltaTime));

    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    {
      BeginMode2D(camera);
      int zero_x = camera.target.x - (screenWidth / 2.0f);
      int zero_y = camera.target.y - (screenHeight / 2.0f);
      DrawTexture(
          bg_tex,
          zero_x + static_cast<int>((screenWidth / 2) - camera.target.x) % 64 -
              (screenWidth / 2 + 64),
          zero_y + static_cast<int>((screenHeight / 2) - camera.target.y) % 64 -
              (screenHeight / 2 + 64),
          Fade(WHITE, 1.0f));
      DrawCircle(0, 0, 5, GetColor(0xea4f36ff));
      DrawText("Congrats! You created your first window!", 190, 200, 20,
               LIGHTGRAY);
      EndMode2D();
    }

    GuiPanel(
        (Rectangle){screenWidth - 180.0f, 10.0f, 170.0f, screenHeight - 20.0f});

    GuiLabel((Rectangle){screenWidth - 175.0f, 15.0f, 160.0f, 25.0f},
             "Civitas WorldGen");
    if (GuiButton((Rectangle){screenWidth - 175.0f, screenHeight - 40.0f,
                              160.0f, 25.0f},
                  "Generate")) {
    }

    width.edit =
        GuiValueBox((Rectangle){screenWidth - 175.0f, 45.0f, 75.0f, 25.0f},
                    nullptr, &width.value, 0, 2048, width.edit)
            ? !width.edit
            : width.edit;
    height.edit =
        GuiValueBox((Rectangle){screenWidth - 90.0f, 45.0f, 75.0f, 25.0f},
                    nullptr, &height.value, 0, 2048, height.edit)
            ? !height.edit
            : height.edit;

    GuiLabel((Rectangle){screenWidth - 175.0f, 95.0f, 160.0f, 25.0f},
             "Settings");
    GuiScrollPanel((Rectangle){screenWidth - 175.0f, 115.0f, 160.0f,
                               screenHeight - 115.0f - 45.0f},
                   (Rectangle){0.0f, 0.0f, 140.0f, 1000.0f}, &scroll);

    mode.edit = GuiDropdownBox(
                    (Rectangle){screenWidth - 175.0f, 75.0f, 160.0f, 25.0f},
                    "Room-Placement;Binary Space Partitioning;Cellular "
                    "Automata;Drunkard's "
                    "Walk;Diffusion Limited Aggregation;Voronoi;Perlin/Simplex "
                    "Noise;Wave Function Collapse;Diamond Square",
                    &mode.value, mode.edit)
                    ? !mode.edit
                    : mode.edit;

    EndDrawing();
  }
};

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

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  GuiLoadStyleDefault();
  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0xc7dcd0ff);
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x2e222fff);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xc7dcd0ff);
  GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x4c9ae6ff);
  GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x313353ff);
  GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x4c9ae6ff);
  GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x8fd3ffff);
  GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x484a77ff);
  GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0x8fd3ffff);
  GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0x9babb2ff);
  GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, 0x3e3546ff);
  GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x9babb2ff);

  Font default_font = GetFontDefault();
  GuiSetFont(default_font);
  GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x2e222fff);

  MainLoop main;

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    main.run();
  }

  CloseWindow();
  return 0;
}
