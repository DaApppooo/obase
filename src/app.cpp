#include "app.hpp"
#include "raylib.h"
#include "raymath.h"
#include "widget.hpp"
#include "sys_settings.hpp"

f32 App::DOUBLE_CLICK_TIME;

void App::init(Own<Widget*> root)
{
  DOUBLE_CLICK_TIME = double_click_time();
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

void App::_events()
{
#define BTN_FUNC(BTN, RL, FUNC) \
  if (IsMouseButton##RL(MOUSE_BUTTON_##BTN)) \
    _src->FUNC(MOUSE_BUTTON_##BTN)
#define BTNS(RL, FUNC) \
  BTN_FUNC(LEFT, RL, FUNC); \
  BTN_FUNC(RIGHT, RL, FUNC); \
  BTN_FUNC(MIDDLE, RL, FUNC)
  BTNS(Pressed, on_click);
  BTNS(Down, on_drag);
  BTNS(Released, on_release);
  if ( 
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
  and double_click_timer <= DOUBLE_CLICK_TIME
  and Vector2Distance(double_click_loc, GetMousePosition()) < DOUBLE_CLICK_MAX_DIST
  ) {
    _src->on_double_click();
    double_click_timer = 1000.f;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
  {
    double_click_loc = GetMousePosition();
    double_click_timer = 0.f;
  }
  double_click_timer += GetFrameTime();
  let scroll = GetMouseWheelMove();
  if (scroll != 0)
    _src->on_scroll();
  let delta = GetMouseDelta();
  if (delta.x != 0 || delta.y != 0)
    _src->on_hover();

  if (focused)
  {
    int key;
    while ((key = GetKeyPressed()))
    {
      assert(key < _keys_pressed.size());
      _keys_pressed[key] = 1;
      focused->on_keydown(KeyboardKey(key));
    }
    for (u32 i = 0; i < _keys_pressed.size(); i++)
    {
      if (_keys_pressed[i] && IsKeyReleased(i))
      {
        _keys_pressed[i] = 0;
        focused->on_keyup(KeyboardKey(i));
      }
    }
  }
}

#define rgb(R, G, B) {R, G, B, 255}
Palette Palette::breeze_dark()
{
  return {
    LoadFont("res/font.ttf"),
    16,
    12,
    {
      rgb(32, 35, 38),
      rgb(61, 174, 233),
      rgb(32, 35, 38),
      rgb(29, 31, 34),
      rgb(41, 44, 48)
    },
    {
      rgb(76,82,89),
      rgb(61,174,233),
      rgb(61,174,233),
      rgb(61,174,233),
      rgb(61,174,233)
    },
    WHITE,
    WHITE,
    rgb(61, 174, 233)
  };
}
Palette Palette::breeze_light()
{
  assert(!"TODO");
  return {};
}

