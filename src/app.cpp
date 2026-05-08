#include "app.hpp"
#include "raylib.h"
#include "widget.hpp"

void App::init(Own<Widget*> root)
{
  _src.reset(root);
}

void App::run(const char* title)
{
  SetTargetFPS(60);
  InitWindow(1600, 900, title);
  _redraw = true;
  IFDEBUG(bool show_debug = false;)
  while (!WindowShouldClose())
  {
    _events();
    IFDEBUG(
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_D))
    {
      show_debug = !show_debug;
      _redraw = true;
    }
    )
    if (!_redraw)
      continue;
    BeginDrawing();
      ClearBackground(BLACK);
      _src->draw();
      IFDEBUG(if (show_debug) _src->debug_draw();)
    EndDrawing();
  }
}


