#pragma once
#include "macros.hpp"
#include "types.hpp"

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
  Color _bg[UI_STATE_COUNT];
  Color _border[UI_STATE_COUNT];
  Color text;
  Color text_selected_fg;
  Color text_selected_bg;

  static Palette breeze_dark();
  static Palette breeze_light();

  Color border(UIState);
  Color bg(UIState);
};

struct App
{
  static macro u32 INIT_W = 1600;
  static macro u32 INIT_H = 900;
  Palette palette;
  uptr<Widget> _src;
  bool _redraw;

  void init(Own<Widget*> root);

  void run(const char* title);

  inline void redraw() { _redraw = true; }

  void _events();
};

App& app();
Palette& palette();

