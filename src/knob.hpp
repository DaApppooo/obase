#pragma once
#include "app.hpp"
#include "widget.hpp"
#include "push_button.hpp"
#include <ostream>

struct Knob : Widget
{
  f32 _x;
  f32 _anim;
  int _auto_dents;
  f32 _custom_dent;
  BaseButtonStyle style;

  Knob(std::string&& name);

  inline f32 value() const { return _anim; }
  inline f32 value(f32 min, f32 max) const { return _anim*(max-min)+min; }
  inline f32 value(f32 x) { _anim = _x = x; app().redraw(); return _x; }
  inline Knob* set_value(f32 x) { _anim = _x = x; app().redraw(); return this; }
  inline Knob* dent(int number_of_dents_between_min_max)
  { _auto_dents = number_of_dents_between_min_max; return this; }
  inline Knob* add_dent(f32 val)
  { _custom_dent = val; return this; }
  
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_drag(MouseButton) override;
  void on_leave() override;
  void on_release(MouseButton) override;
  void on_scroll() override;

  void help(std::ostream& out) override;
  void update() override;
  void draw() override;
  ~Knob() override;
};
