#include "app.hpp"
#include "raylib.h"
#include "raymath.h"
#include "widget.hpp"
#include "sys_settings.hpp"
#include "rlgl.h"
#include "raymath.h"
#include <cassert>
#include <string_view>
#include <unistd.h>

f32 App::DOUBLE_CLICK_TIME;
App* _obase_current_app = nullptr;

void focus(Widget *w)
{
  if (w == app().focused)
    return;
  if (app().focused)
  {
    if (w)
    {
      Widget* p = w->parent;
      while (p != nullptr)
      {
        if (p == app().focused)
          break;
        p = p->parent;
      }
      if (p == nullptr)
        app().focused->on_unfocus();
    }
    else
      app().focused->on_unfocus();
  }
  app().focused = w;
}

Widget* App::request(std::string_view name)
{
  if (root->_name == name)
    return root.get();
  return root->request(name);
}

void App::init(Own<Widget*> root)
{
  DOUBLE_CLICK_TIME = double_click_time();
  focused = nullptr;
  this->root.reset(root);
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
  palette.compute_largest_digit();
  SetTextureFilter(palette.font.texture, TEXTURE_FILTER_TRILINEAR);
  assert(palette.font.recs != nullptr);
  IFDEBUG(bool show_debug = false;)
  
  while (!WindowShouldClose())
  {
    _events();
    root->update();
    IFDEBUG(
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_D))
    {
      show_debug = !show_debug;
    }
    )
    BeginDrawing();
    ClearBackground(ColorBrightness(palette.bg(UI_ACTIVE), -0.1f));
    root->draw();
    if (focused)
      focused->draw(); // redraw focused over everything. not ideal but it works
    IFDEBUG(if (show_debug) root->debug_draw();)
    EndDrawing();
  }

  UnloadFont(palette.font);
  for (Texture& tex : palette.icons)
    UnloadTexture(tex);

  CloseWindow();
}

void App::_events()
{
#define GENERAL_BTN_FUNC(PTR, BTN, RL, FUNC) \
  if (PTR && IsMouseButton##RL(MOUSE_BUTTON_##BTN)) \
    PTR->FUNC(MOUSE_BUTTON_##BTN)
#define BTN_FUNC(BTN, RL, FUNC) GENERAL_BTN_FUNC(src, BTN, RL, FUNC)
#define FOCUS_BTN_FUNC(BTN, RL, FUNC) GENERAL_BTN_FUNC(focused, BTN, RL, FUNC)
#define BTNS(RL, FUNC) \
  BTN_FUNC(LEFT, RL, FUNC); \
  BTN_FUNC(RIGHT, RL, FUNC); \
  BTN_FUNC(MIDDLE, RL, FUNC)
#define FOCUSED_BTNS(RL, FUNC) \
  FOCUS_BTN_FUNC(LEFT, RL, FUNC); \
  FOCUS_BTN_FUNC(RIGHT, RL, FUNC); \
  FOCUS_BTN_FUNC(MIDDLE, RL, FUNC)
  // tell the compiler, hey, the pointer won't change between calls:
  Widget* const src = root.get();
  Widget* const old_focus = focused;
  BTNS(Pressed, on_click);
  BTNS(Down, on_drag);
  BTNS(Released, on_release);
  let screen_w = GetScreenWidth(), screen_h = GetScreenHeight();
  if (root->rect.width != screen_w || root->rect.height != screen_h)
  {
    current_scissor = {0, 0, f32(screen_w), f32(screen_h)};
  }
  root->rect = {0, 0, f32(screen_w), f32(screen_h)};
  let mpo = GetMousePosition();
  if ( 
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
  and double_click_timer <= DOUBLE_CLICK_TIME
  and Vector2Distance(double_click_loc, mpo) < DOUBLE_CLICK_MAX_DIST
  ) {
    root->on_double_click();
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
    root->on_scroll();
  let delta = GetMouseDelta();
  if (delta.x != 0 || delta.y != 0)
    root->on_hover();

  // send input to focused element, except if it's just been focused because
  // in this case it should already have recieved inputs
  if (focused and focused == old_focus)
  {
    // focused however can change !
    FOCUSED_BTNS(Pressed, on_click);
    FOCUSED_BTNS(Down, on_drag);
    FOCUSED_BTNS(Released, on_release);
    if (focused && CheckCollisionPointRec(mpo, focused->rect))
    {
      if (delta.x != 0 || delta.y != 0)
      {
        focused->on_hover();
      }
      if (
          IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
      and double_click_timer <= DOUBLE_CLICK_TIME
      and Vector2Distance(double_click_loc, mpo) < DOUBLE_CLICK_MAX_DIST
      ) {
        focused->on_double_click();
        double_click_timer = 1000.f;
      }
    }

    if (!focused)
      return;
    
    int key;
    while ((key = GetKeyPressed()))
    {
      assert(key < _keys_pressed.size());
      _keys_pressed[key] = 1;
      focused->on_keydown(from_keycode(KeyboardKey(key)));
    }
    for (u32 i = 0; i < _keys_pressed.size(); i++)
    {
      if (_keys_pressed[i] && IsKeyReleased(i))
      {
        _keys_pressed[i] = 0;
        focused->on_keyup(from_keycode(KeyboardKey(i)));
      }
    }
  }
}

void Palette::draw_icon(Icon icon, Rect fit, f32 angle, Color tint)
{
  if (fit.width > fit.height)
  {
    fit.x += (fit.width - fit.height)/2;
    fit.width = fit.height;
  }
  else
  {
    fit.y += (fit.height - fit.width)/2;
    fit.height = fit.width;
  }
  let center = Vec2{fit.width/2.f, fit.height/2.f};
  DrawTexturePro(
    icons[icon],
    {0.f, 0.f, f32(icons[icon].width), f32(icons[icon].height)},
    offset(fit, center),
    center,
    angle,
    tint
  );
}

#define rgb(R, G, B) {R, G, B, 255}
Palette Palette::breeze_dark()
{
  return {
    LoadFontEx("res/font.ttf", 20, nullptr, 0),
    0, // largest_digit_w (set automatically)
    0, // dot_w (set automatically)
    30, // title_size
    20, // text_size
    { // bg
      rgb(32, 35, 38), // UI_DEFAULT
      rgb(61, 174, 233), // UI_SELECTED
      rgb(32, 35, 38), // UI_INACTIVE
      rgb(19, 21, 24), // UI_ACTIVE
      rgb(41, 44, 48) // UI_FOCUSED
    },
    { // border
      rgb(76,82,89), // UI_DEFAULT
      rgb(61,174,233), // UI_SELECTED
      rgb(11, 12, 13), // UI_INACTIVE
      rgb(106,112,119), // UI_ACTIVE
      rgb(61,174,233) // UI_FOCUSED
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

void Palette::compute_largest_digit()
{
  const char* digits[] = {"0","1","2","3","4","5","6","7","8","9"};
  for (const char* s : digits)
  {
    let size = MeasureTextEx(font, s, text_size, 2.f).x;
    if (size > largest_digit_w)
      largest_digit_w = size;
  }
  dot_w = MeasureTextEx(font, ".", text_size, 2.f).x;
}



