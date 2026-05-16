#pragma once
#include "app.hpp"
#include "types.hpp"
#include "widget.hpp"
#include "push_button.hpp"
#include <ostream>

struct NumBox : Widget
{
  static macro f32 padding = 5.f;
  f32 _x;
  f32 _anim;
  f32 min, max;
  BaseButtonStyle style;
  i32 pow_min, pow_max;

  NumBox(std::string&& id);
  
  // between [0, 1]
  inline f32 value() const { return _anim; }
  inline f32 value(f32 min, f32 max) const { return _anim*(max-min)+min; }
  // between [min, max]
  inline f32 value_scaled() const
  { return value() * (max - min) + min; }
  inline NumBox* set_value(f32 x)
  { _anim = _x = x; app().redraw(); return this; }
  inline NumBox* range(f32 min_, f32 max_)
  { min=min_; max=max_; return this; }
  // Minimum and maximum power of 10 for the display of the internal value.
  inline NumBox* format(i32 pow_min_, i32 pow_max_)
  { pow_min=pow_min_; pow_max=pow_max_; return this; }

  void on_click(MouseButton) override;
  void on_hover() override;
  void on_drag(MouseButton) override;
  void on_leave() override;
  void on_release(MouseButton) override;
  void on_scroll() override;

  void help(std::ostream& out) override;
  void draw() override;
  
  inline f32 _font_size() const
  { return (rect.height-2.f*padding)/palette().text_size; }
  inline i32 digit_power(f32 relative_x) const;
  inline f32 digit_pos(i32 digit_power) const;
};


// inline i32 NumBox::digit_power(f32 relative_x) const
// {
//   let& pal = palette();
//   let digit_size = (rect.height-padding*2.f)/pal.text_size*pal.largest_digit_w;
//   let available_space = rect.width-padding*2.f;
  
// }

