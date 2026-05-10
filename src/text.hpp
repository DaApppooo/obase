#pragma once
#include "widget.hpp"
#include "anchor.hpp"
#include "app.hpp"
#include "raylib.h"

struct Text : Widget
{
  static constexpr f32 SPACING = 2.f;
  std::string _text;
  AnchorPoint _rect_anchor;
  u32 select_start, select_end;

  Text(std::string&& txt);

  inline Text* anchor(AnchorPoint where)
  { _rect_anchor = where; return this; }
  inline Text* selectable()
  { select_start = select_end = 0; return this; }
  [[nodiscard]]
  inline bool is_selectable() const
  { return select_end < select_start; }

  inline const std::string& text() const { return _text; }
  inline const std::string& text(std::string_view new_value)
  {
    _text.clear();
    _text += new_value;
    return _text;
  }

  void on_click(MouseButton) override;
  void on_drag(MouseButton) override;
  void on_double_click() override;
  void update() override;
  void draw() override;
  ~Text() override;
};
