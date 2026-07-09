#pragma once
#include "app.hpp"
#include "push_button.hpp"
#include "widget.hpp"
#include "raymath.h"

struct Slider : Widget
{
  f32 _x;
  f32 _anim;
  int _auto_dents;
  f32 _custom_dent;
  Orientation _orient;
  BaseButtonStyle style;

  Slider(std::string&& name);

  inline f32 value() const { return _anim; }
  inline f32 value(f32 min, f32 max) const { return _anim*(max-min)+min; }
  inline f32 value(f32 x) { _anim = _x = x; return _x; }
  inline Slider* set_value(f32 x) { _anim = _x = x; return this; }
  inline Slider* dent(int number_of_dents_between_min_max)
  { _auto_dents = number_of_dents_between_min_max; return this; }
  inline Slider* add_dent(f32 val)
  { _custom_dent = val; return this; }
  inline Slider* orient(Orientation o)
  { _orient = o; return this; }
  
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_drag(MouseButton) override;
  void on_leave() override;
  void on_release(MouseButton) override;
  void on_scroll() override;

  void help(std::ostream& out) override;
  void update() override;
  void draw() override;
  ~Slider() override;

  inline f32 _handle_radius() const
  {
    if (_orient == HORIZONTAL)
      return h()/2.f;
    else
      return w()/2.f;
  }
  inline Vec2 _groove_max() const
  {
    if (_orient == HORIZONTAL)
      return { rect.x + rect.width - _handle_radius(), rect.y + _handle_radius() };
    else
      return { rect.x + _handle_radius(), rect.y + _handle_radius() };      
  }
  inline Vec2 _groove_min() const
  {
    if (_orient == HORIZONTAL)
      return { rect.x + _handle_radius(), rect.y + _handle_radius() };
    else
      return { rect.x + _handle_radius(), rect.y + rect.height - _handle_radius() };      
  }
  inline Vec2 _pos(f32 t) const
  { return _groove_min()*(1.f-t) + _groove_max()*t; }
  inline Vec2 _handle_pos() const
  { return _pos(_anim); }
  inline Vec2 _orth() const
  {
    if (_orient == HORIZONTAL)
      return {0.f, 1.f};
    else
      return {1.f, 0.f};
  }
  inline f32 _secondary_size()
  {
    if (_orient == HORIZONTAL)
      return h()/2.f;
    else
      return w()/2.f;
  }
};


