#pragma once
#include "graphics.hpp"
#include "macros.hpp"
#include "raylib.h"
#include "types.hpp"
#include "sys_settings.hpp"
#include <bitset>
#include <string_view>

#define EXPAND_RECT(RECT) (RECT).x, (RECT).y, (RECT).width, (RECT).height

struct Widget;

enum UIState
{
  UI_DEFAULT,
  UI_SELECTED,
  UI_INACTIVE,
  UI_ACTIVE,
  UI_FOCUSED,
  UI_STATE_COUNT
};

inline void fit_rect_inside(Rect& fit_me, Rect inside_me)
{
  if (fit_me.x < inside_me.x)
  {
    fit_me.width -= inside_me.x - fit_me.x;
    fit_me.x = inside_me.x;
  }
  if (fit_me.y < inside_me.y)
  {
    fit_me.height -= inside_me.y - fit_me.y;
    fit_me.y = inside_me.y;
  }
  if (fit_me.x+fit_me.width > inside_me.x+inside_me.width)
    fit_me.width = inside_me.x+inside_me.width-fit_me.x;
  if (fit_me.y+fit_me.height > inside_me.y+inside_me.height)
    fit_me.height = inside_me.y+inside_me.height-fit_me.y;
}

struct Palette
{
  Font font;
  u32 largest_digit_w;
  u32 dot_w;
  u32 title_size;
  u32 text_size;
  Color _bg[UI_STATE_COUNT];
  Color _border[UI_STATE_COUNT];
  Color text;
  Color text_selected_fg;
  Color text_selected_bg;
  IconBuffer icons;

  static Palette breeze_dark();
  static Palette breeze_light();

  inline Color border(UIState state) const { return _border[state]; }
  inline Color bg(UIState state) const { return _bg[state]; }

  void compute_largest_digit();
  
  // angle is degrees
  void draw_icon(Icon icon, Rect fit, f32 angle, Color tint);
};

struct App
{
  static macro u32 INIT_W = 1600;
  static macro u32 INIT_H = 900;
  static macro f32 DOUBLE_CLICK_MAX_DIST = 10.f;
  static f32 DOUBLE_CLICK_TIME;
  std::bitset<512> _keys_pressed;
  Palette palette;
  uptr<Widget> root;
  Widget* focused;
  Vec2 double_click_loc;
  f32 double_click_timer;
  Rect current_scissor;
  std::vector<Widget*> _gfx_widgets;

  App() = default;

  void init(Own<Widget*> root);

  void run(const char* title);

  Widget* request(std::string_view name);
  inline Widget* request(const char* s) { return request(std::string_view(s)); }

  [[nodiscard]]
  inline Rect scissor_begin(Rect new_scissor, bool fit_inside_previous = true)
  {
    Rect temp = current_scissor;
    EndScissorMode();
    if (fit_inside_previous)
      fit_rect_inside(new_scissor, current_scissor);
    current_scissor = new_scissor;
    BeginScissorMode(EXPAND_RECT(new_scissor));
    return temp;
  }
  inline void scissor_end(Rect old_scissor)
  {
    EndScissorMode();
    current_scissor = old_scissor;
    BeginScissorMode(EXPAND_RECT(old_scissor));
  }
  inline void set_cursor(MouseCursor new_cursor)
  {
    SetMouseCursor(new_cursor);
  }

  void _events();
};

inline App& app()
{
  extern App* _obase_current_app;
#ifdef _DEBUG
  if (!_obase_current_app)
    assert(!"You forgot to initialize your app.");
#endif
  return *_obase_current_app;
}
inline Palette& palette()
{ return app().palette; }
inline Font& font()
{ return app().palette.font; }
void focus(Widget* w);
inline void unfocus(Widget* w)
{ if (!w) return; if (app().focused == w) focus(nullptr); }
// Checks whether the given widget is focused or not.
[[nodiscard]] inline bool is_focused(Widget* w)
{ return app().focused == w; }
#define FOCUS_ME focus(this)
#define UNFOCUS_ME unfocus(this)

inline void DrawRoundedBox(
  Rect outer,
  f32 roundness_left,
  f32 roundness_right,
  f32 border_thickness,
  Color border,
  Color bg,
  i32 segments = 8
) {
  DrawRectangleRoundedPro(
    outer,
    roundness_left,
    roundness_right,
    segments,
    border
  );
  DrawRectangleRoundedPro(
    reduce(outer, border_thickness),
    roundness_left,
    roundness_right,
    segments,
    bg
  );
}

#define TEX_ORIGINAL_RECT(tex) Rect{ 0.f, 0.f, f32(tex.width), f32(tex.height) }

