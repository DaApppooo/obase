#pragma once
#include "macros.hpp"

using WidgetFlag = u32;
// The widget can be focused (ex: for keyboard actions).
macro WidgetFlag FOCUSABLE   = 0b00010;
// The widget can be resized by the user in the X direction
macro WidgetFlag X_RESIZABLE = 0b00100;
// The widget can be resized by the user in the X direction
macro WidgetFlag Y_RESIZABLE = 0b01000;

struct Widget
{
  Widget* parent;
#ifdef _DEBUG
  std::string _name;
#endif
  Rect rect;

  Widget(std::string&& name);

  inline f32 x() const { return rect.x; }
  inline f32 y() const { return rect.y; }
  inline f32 w() const { return rect.width; }
  inline f32 h() const { return rect.height; }
  inline f32 x(float v) { return rect.x = v; }
  inline f32 y(float v) { return rect.y = v; }
  inline f32 w(float v) { return rect.width = v; }
  inline f32 h(float v) { return rect.height = v; }
  
  inline f32 left() const { return x(); }
  inline f32 top() const { return y(); }
  inline f32 right() const { return x()+w(); }
  inline f32 bottom() const { return y()+h(); }
  inline f32 left(f32 v) { return x(v); }
  inline f32 top(f32 v) { return y(v); }
  inline f32 right(f32 v) { return w(v-x()); }
  inline f32 bottom(f32 v) { return h(v-y()); }
  
  virtual void debug_draw();

  // Called when the mouse buttons are up and the mouse is over the rect.
  virtual void on_hover();
  // Called when a mouse button was just pressed and the mouse is over the rect.
  virtual void on_click();
  // Called when a mouse button is down and the mouse is over the rect.
  virtual void on_drag();
  // Called when a mouse button just got released and the mouse is over the rect.
  virtual void on_release();
  // Called when the widget is focused (if focusable) and a key just got pressed.
  virtual void on_keydown();
  // Called when the widget is focused (if focusable) and a key just got released.
  virtual void on_keyup();
  
  // Called only when redrawing is necessary.
  // To ask for a redraw you can use app().redraw()
  virtual void draw() = 0;
  // Called aounrd 60 times a second.
  virtual void update() = 0;
  virtual ~Widget() = 0;
};



