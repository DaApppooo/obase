#pragma once
#include "macros.hpp"
#include "raylib.h"
#include "types.hpp"
#include <bitset>
#include <string_view>

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

struct Palette
{
  Font font;
  u32 title_size;
  u32 text_size;
  Color _bg[UI_STATE_COUNT];
  Color _border[UI_STATE_COUNT];
  Color text;
  Color text_selected_fg;
  Color text_selected_bg;

  static Palette breeze_dark();
  static Palette breeze_light();

  inline Color border(UIState state) { return _border[state]; }
  inline Color bg(UIState state) { return _bg[state]; }

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
  bool _redraw;

  App() = default;

  void init(Own<Widget*> root);

  void run(const char* title);

  inline void redraw() { _redraw = true; }

  void _events();
};

App& app();
Palette& palette();
Font& font();
void focus(Widget*);
#define FOCUS_ME focus(this)
