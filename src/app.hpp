#pragma once
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

  inline Color border(UIState state) { return _border[state]; }
  inline Color bg(UIState state) { return _bg[state]; }

  void compute_largest_digit();
  
  void draw_text(std::string_view v, Vec2 pos);
  void draw_text_selected(std::string_view v, Vec2 pos);
  void draw_text(const char* v, Vec2 pos);
  void draw_text_selected(const char* v, Vec2 pos);
  void draw_title(std::string_view v, Vec2 pos);
  void draw_title_selected(std::string_view v, Vec2 pos);
  void draw_title(const char* v, Vec2 pos);
  void draw_title_selected(const char* v, Vec2 pos);
};

struct App
{
  static macro u32 INIT_W = 1600;
  static macro u32 INIT_H = 900;
  static macro f32 DOUBLE_CLICK_MAX_DIST = 10.f;
  static f32 DOUBLE_CLICK_TIME;
  std::bitset<512> _keys_pressed;
  Palette palette;
  uptr<Widget> _src;
  Widget* focused;
  Vec2 double_click_loc;
  f32 double_click_timer;
  int _redraw;
  Rect current_scissor;

  App() = default;

  void init(Own<Widget*> root);

  void run(const char* title);

  Widget* request(std::string_view name);

  inline Rect scissor_begin(Rect new_scissor)
  {
    Rect temp = current_scissor;
    EndScissorMode();
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
  inline void redraw() { if (_redraw > 1) _redraw = 0; }

  void _events();
};

App& app();
Palette& palette();
Font& font();
void focus(Widget*);
#define FOCUS_ME focus(this)
#define UNFOCUS_ME if (app().focused == this) focus(nullptr)
