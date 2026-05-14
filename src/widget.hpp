#pragma once
#include "anchor.hpp"
#include "macros.hpp"
#include "raylib.h"

using WidgetFlag = u32;
// The widget width is locked.
macro WidgetFlag W_LOCKED    = 0b0000001;
// The widget height is locked.
macro WidgetFlag H_LOCKED    = 0b0000010;
// The widget can be resized by the user in the X direction
macro WidgetFlag X_RESIZABLE = 0b0000100;
// The widget can be resized by the user in the Y direction
macro WidgetFlag Y_RESIZABLE = 0b0001000;


enum Orientation
{
  HORIZONTAL,
  VERTICAL
};

struct Widget
{
  Anchor anchors[4];
  Widget* parent;
  std::string _name;
  Rect rect;
  WidgetFlag flags;
  u32 anchor_p;

  Widget(std::string&& name);

  inline Widget* add_flag(WidgetFlag flag) { flags |= flag; return this; }

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
  inline f32 right(f32 v) { return x(v-w()); }
  inline f32 bottom(f32 v) { return y(v-h()); }

  inline Anchor& push_anchor()
  { assert(anchor_p < 4); return anchors[anchor_p++]; }
  inline Widget* set_size(Vec2 wh)
  { rect.width = wh.x; rect.height = wh.y; return this; }
  inline Widget* place(Rect rect_) { rect = rect_; return this; }
  inline Widget* place(f32 x, f32 y, f32 w, f32 h) { return place({x,y,w,h}); }
  inline Widget* size(Vec2 size) { w(size.x); h(size.y); return this; }
  inline Widget* size(f32 w, f32 h) { size({w, h}); return this; }
  inline Widget* size(f32 side) { size({side, side}); return this; }
  
  inline Widget* fill_parent(float inset = 0.f)
  {
    push_anchor().anchor(this, TOP_LEFT).on(parent, TOP_LEFT).resizable()
      .margin(inset);
    push_anchor().anchor(this, BOTTOM_RIGHT).on(parent, BOTTOM_RIGHT).resizable()
      .margin(inset);
    return this;
  }
  inline Widget* center_in_parent()
  {
    push_anchor().anchor(this, CENTER).on(parent, CENTER);
    return this;
  }
  
  // PUT NEXT TO OTHER
  inline Widget* put_under(Widget* other, AnchorPoint align, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, TOP).on(other, BOTTOM).margin(margin);
    push_anchor().anchor(this, align).on(other, align);
    return this;
  }
  inline Widget* put_above(Widget* other, AnchorPoint align, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, BOTTOM).on(other, TOP).margin(margin);
    push_anchor().anchor(this, align).on(other, align);
    return this;
  }
  inline Widget* left_of(Widget* other, AnchorPoint align, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, RIGHT).on(other, LEFT).margin(margin);
    push_anchor().anchor(this, align).on(other, align);
    return this;
  }
  inline Widget* right_of(Widget* other, AnchorPoint align, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, LEFT).on(other, RIGHT).margin(margin);
    push_anchor().anchor(this, align).on(other, align);
    return this;
  }
  
  // PUT INSIDE OTHER
  inline Widget* top_in(Widget* other, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, TOP).on(other, TOP).margin(margin);
    return this;
  }
  inline Widget* bottom_in(Widget* other, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, BOTTOM).on(other, BOTTOM).margin(margin);
    return this;
  }
  inline Widget* left_in(Widget* other, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, LEFT).on(other, LEFT).margin(margin);
    return this;
  }
  inline Widget* right_in(Widget* other, f32 margin = 0)
  {
    assert(other);
    push_anchor().anchor(this, RIGHT).on(other, RIGHT).margin(margin);
    return this;
  }
  
  inline Widget* between_x(Widget* left, Widget* right, f32 margin = 0)
  {
    assert(left);
    assert(right);
    push_anchor()
      .anchor(this, LEFT)
      .on(left, RIGHT)
      .margin(margin)
      .resizable();
    push_anchor()
      .anchor(this, RIGHT)
      .on(right, LEFT)
      .margin(margin)
      .resizable();
    return this;
  }
  inline Widget* between_y(Widget* under, Widget* above, f32 margin = 0)
  {
    assert(under);
    assert(above);
    push_anchor()
      .anchor(this, TOP)
      .on(above, BOTTOM)
      .margin(margin)
      .resizable();
    push_anchor()
      .anchor(this, BOTTOM)
      .on(under, TOP)
      .margin(margin)
      .resizable();
    return this;
  }
  inline Widget* between(
    Widget* left, Widget* right,
    Widget* under, Widget* above,
    f32 margin = 0
  ) {
    assert(under);
    assert(above);
    push_anchor()
      .anchor(this, TOP)
      .on(above, BOTTOM)
      .margin(margin)
      .resizable();
    push_anchor()
      .anchor(this, BOTTOM)
      .on(under, TOP)
      .margin(margin)
      .resizable();
    assert(left);
    assert(right);
    push_anchor()
      .anchor(this, LEFT)
      .on(left, RIGHT)
      .margin(margin)
      .resizable();
    push_anchor()
      .anchor(this, RIGHT)
      .on(right, LEFT)
      .margin(margin)
      .resizable();
    return this;
  }
  
  
  // Search for a child named 'name'. If not found, returns nullptr.
  virtual Widget* request(std::string_view name);
  
  virtual void debug_draw();

  // Called when the mouse buttons are up and the mouse is over the rect.
  virtual void on_hover();
  // Called when the mouse buttons are up and the mouse was over the rect.
  virtual void on_leave();
  // Called when a mouse button was just pressed and the mouse is over the rect.
  virtual void on_click(MouseButton);
  // Called when a double click was done with the mouse is over the rect.
  virtual void on_double_click();
  // Called when a mouse button is down and the mouse is over the rect.
  virtual void on_drag(MouseButton);
  // Called when a mouse button just got released and the mouse is over the rect.
  virtual void on_release(MouseButton);
  // Called when the scroll wheel moved and the mouse is over the rect.
  virtual void on_scroll();
  // Called when the widget is focused (if focusable) and a key just got pressed.
  virtual void on_keydown(KeyboardKey key);
  // Called when the widget is focused (if focusable) and a key just got released.
  virtual void on_keyup(KeyboardKey key);
  
  // Called only when redrawing is necessary.
  // To ask for a redraw you can use app().redraw()
  virtual void draw() = 0;
  // Called around 60 times a second.
  virtual void update();
  virtual ~Widget() = 0;
};

FORMATTER(Widget, [](std::ostream& out, const Widget& ptr)
{
  out << "Widget*[at " << (void*)&ptr
      << "](" << ptr._name
      << ", parent=" << (void*)ptr.parent
      << ", rect=" << std::format("{}", ptr.rect)
      << ")";
});



