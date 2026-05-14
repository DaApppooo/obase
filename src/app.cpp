#include "app.hpp"
#include "raylib.h"
#include "raymath.h"
#include "widget.hpp"
#include "sys_settings.hpp"
#include "rlgl.h"
#include <cassert>
#include <string_view>
#include <unistd.h>

f32 App::DOUBLE_CLICK_TIME;
App* _obase_current_app = nullptr;
App& app()
{
#ifdef _DEBUG
  if (!_obase_current_app)
    assert(!"You forgot to initialize your app.");
#endif
  return *_obase_current_app;
}
Palette& palette()
{
#ifdef _DEBUG
  if (!_obase_current_app)
    assert(!"You forgot to initialize your app.");
#endif
  return _obase_current_app->palette;
}
Font& font()
{
#ifdef _DEBUG
  if (!_obase_current_app)
    assert(!"You forgot to initialize your app.");
#endif
  return _obase_current_app->palette.font;
}
void focus(Widget* w)
{ app().focused = w; }


Widget* App::request(std::string_view name)
{
  return _src->request(name);
}

void App::init(Own<Widget*> root)
{
  DOUBLE_CLICK_TIME = double_click_time();
  focused = nullptr;
  _src.reset(root);
  _obase_current_app = this;
  current_scissor = {0, 0, 1600, 900};
}

void App::run(const char* title)
{
  SetTargetFPS(60);
  SetConfigFlags(
    FLAG_WINDOW_RESIZABLE
    | FLAG_MSAA_4X_HINT
  );
  InitWindow(1600, 900, title);
  // The font must be loaded after alling InitWindow !!!
  if (system_theme() == THEME_DARK)
    palette = Palette::breeze_dark();
  else
    palette = Palette::breeze_light();
  load_icons(palette.icons);
  assert(palette.font.recs != nullptr);
  _redraw = -5;
  IFDEBUG(bool show_debug = false;)
  while (!WindowShouldClose())
  {
    _events();
    _src->update();
    IFDEBUG(
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_D))
    {
      show_debug = !show_debug;
      _redraw = true;
    }
    )
    BeginDrawing();
    if (_redraw < 10)
    {
      _redraw++;
      ClearBackground(palette.bg(UI_ACTIVE));
      _src->draw();
      IFDEBUG(if (show_debug) _src->debug_draw();)
    }
    EndDrawing();
  }

  UnloadFont(palette.font);
  for (Texture& tex : palette.icons)
    UnloadTexture(tex);

  CloseWindow();
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
  let screen_w = GetScreenWidth(), screen_h = GetScreenHeight();
  if (_src->rect.width != screen_w || _src->rect.height != screen_h)
    redraw();
  _src->rect = {0, 0, f32(screen_w), f32(screen_h)};
  let mpo = GetMousePosition();
  if ( 
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
  and double_click_timer <= DOUBLE_CLICK_TIME
  and Vector2Distance(double_click_loc, mpo) < DOUBLE_CLICK_MAX_DIST
  ) {
    _src->on_double_click();
    double_click_timer = 1000.f;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
  {
    double_click_loc = mpo;
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
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      focused->on_drag(MOUSE_BUTTON_LEFT);
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
      focused->on_drag(MOUSE_BUTTON_RIGHT);
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
      focused->on_drag(MOUSE_BUTTON_MIDDLE);

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
      focused->on_release(MOUSE_BUTTON_LEFT);
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
      focused->on_release(MOUSE_BUTTON_RIGHT);
    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
      focused->on_release(MOUSE_BUTTON_MIDDLE);
    
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
    LoadFontEx("res/font.ttf", 20, nullptr, 0),
    30,
    20,
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



