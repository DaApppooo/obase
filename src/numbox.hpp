#pragma once
#include "app.hpp"
#include "types.hpp"
#include "widget.hpp"
#include "push_button.hpp"
#include <ostream>

struct NumBox : Widget
{
  static macro f32 padding = 5.f;
  f64 _x; // this time _x is in [min, max]
  f64 min, max;
  BaseButtonStyle style;
  i32 pow_min, pow_max;
  i32 _pow_sel; ///< Selected power.

  NumBox(std::string&& id);
  
  // between [0, 1]
  inline f32 value() const { return (_x-min)/(max-min); }
  inline f32 value(f32 min, f32 max) const { return _x*(max-min)+min; }
  // between [min, max]
  inline f32 value_scaled() const
  { return _x; }
  inline NumBox* set_value(f32 x)
  { _x = x; app().redraw(); return this; }
  inline NumBox* range(f64 min_, f64 max_)
  { min=min_; max=max_; _x = std::clamp(_x, min, max); return this; }
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
  void update() override;
  void draw() override;
  
  inline f32 _font_size() const
  { return rect.height-2.f*padding; }
  // Relative to the widget rect.
  inline i32 digit_power(f32 relative_x) const;
  inline f32 digit_pos(i32 digit_power) const;
};

inline i32 NumBox::digit_power(f32 relative_x) const
{
  let& pal = palette();
  let digit_size = _font_size()/pal.text_size*pal.largest_digit_w;
  let available_space = rect.width-padding*2.f;
  let dot_size = _font_size()/pal.text_size*pal.dot_w;
  let nondec = std::max(pow_max, 0) + 1;
  relative_x -= padding;
  if (relative_x < nondec*digit_size)
    return pow_max - i32(relative_x/digit_size);
  relative_x -= nondec*digit_size - dot_size;
  return -i32(relative_x/digit_size);
}

inline f32 NumBox::digit_pos(i32 digit_power) const
{
  let& pal = palette();
  let digit_size = _font_size()/pal.text_size*pal.largest_digit_w;
  let dot_size = _font_size()/pal.text_size*pal.dot_w;
  return (pow_max - digit_power) * digit_size
         + ((digit_power < 0) ? dot_size : 0);
}

