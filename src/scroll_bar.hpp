#pragma once
#include "macros.hpp"
#include "raylib.h"
#include "types.hpp"
#include "widget.hpp"
#include "push_button.hpp"
#include "raymath.h"

struct ScrollBarStyle
{
  static macro f32 BUTTON_MIN_SIZE = 20.f;
  static macro f32 HANDLE_MIN_SIZE = 20.f;
  
  Rect old_total_rect;
  Rect handle;
  Vec2 old_mpo;
  BaseButtonStyle min_btn_style;
  BaseButtonStyle max_btn_style;
  BaseButtonStyle handle_style;
  Orientation orient;

  inline f32& xy(Rect& r) const { return orient == HORIZONTAL ? r.x : r.y; }
  inline f32& xy(Vec2& r) const { return orient == HORIZONTAL ? r.x : r.y; }
  inline f32& wh(Rect& r) const
  { return orient == HORIZONTAL ? r.width : r.height; }
  inline f32 xy(const Rect& r) const { return orient == HORIZONTAL ? r.x : r.y; }
  inline f32 xy(const Vec2& r) const { return orient == HORIZONTAL ? r.x : r.y; }
  inline f32 wh(const Rect& r) const
  { return orient == HORIZONTAL ? r.width : r.height; }
  inline Rect groove(Rect total) const
  {
    if (orient == HORIZONTAL)
    {
      return {
        total.x + BUTTON_MIN_SIZE,
        total.y,
        total.width - 2.f*BUTTON_MIN_SIZE,
        total.height
      };
    }
    else
    {
      return {
        total.x,
        total.y + BUTTON_MIN_SIZE,
        total.width,
        total.height- 2.f*BUTTON_MIN_SIZE
      };
    }
  }
  // scaled between 0 and 1
  inline f32 value(Rect total) const
  {
    mut x = (xy(handle) - xy(total)) / (wh(groove(total)) - wh(handle));
    if (orient == VERTICAL)
      x = 1.f - x;
    return x;
  }
  // scaled between 0 and 1 (so you can't use it just like that as a zoom)
  inline f32 zoom(Rect total) const
  {
    return (wh(handle) - HANDLE_MIN_SIZE) / (wh(groove(total)) - HANDLE_MIN_SIZE);
  }
  inline Rect min_btn(Rect total) const
  {
    if (orient == HORIZONTAL)
      return { total.x, total.y, BUTTON_MIN_SIZE, total.height };
    else
      return {
        total.x, total.y+total.height-BUTTON_MIN_SIZE,
        total.width, BUTTON_MIN_SIZE
      };
  }
  inline Rect max_btn(Rect total) const
  {
    if (orient == HORIZONTAL)
      return {
        total.x+total.width-BUTTON_MIN_SIZE,
        total.y, BUTTON_MIN_SIZE, total.height
      };
    else
      return {
        total.x, total.y,
        total.width, BUTTON_MIN_SIZE
      };
  }
  inline void value(Rect total, f32 x)
  {
    if (orient == HORIZONTAL)
    {
      handle.x = x * (wh(groove(total)) - wh(handle)) + xy(total);
    }
    else
    {
      handle.y = (1.f - x) * (wh(groove(total)) - wh(handle)) + xy(total);
    }
  }
  // the side of the handle pointing towards the max value
  inline f32 handle_max() const
  { return orient == HORIZONTAL ? handle.x+handle.width : handle.y; }
  // the side of the handle pointing towards the max value.
  // will rescale the handle.
  // x is the screen position.
  inline void handle_max(f32 x)
  {
    if (orient == HORIZONTAL)
      handle.width = x - handle.x;
    else
      handle.height += x - handle.y,
      handle.y = x;
  }
  // the side of the handle pointing towards the min value
  inline f32 handle_min() const
  { return orient == HORIZONTAL ? handle.x : handle.y+handle.height; }
  // the side of the handle pointing towards the min value.
  // will rescale the handle.
  // x is the screen position. Use style.xy(my_position) to get the 
  inline void handle_min(f32 x)
  {
    if (orient == HORIZONTAL)
      handle.width += x - handle.x,
      handle.x = x;
    else
      handle.height = x - handle.y;
  }
  inline f32 groove_min(Rect total) const
  {
    return orient == HORIZONTAL
          ? groove(total).x
          : groove(total).y+groove(total).height;
  }
  inline f32 groove_max(Rect total) const
  {
    return orient == HORIZONTAL
            ? groove(total).x+groove(total).width
            : groove(total).y;
  }
  
  inline void clamp(Rect total)
  {
    let groo = groove(total);
    if (orient == HORIZONTAL)
    {
      handle.y = total.y;
      handle.height = total.height;
      if (handle.width > groo.width)
        handle.width = groo.width;
      if (handle.x < groo.x)
        handle.x = groo.x;
      if (handle.x+handle.width > groo.x+groo.width)
        handle.x = groo.x+groo.width-handle.width;
    }
    else
    {
      handle.x = total.x;
      handle.width = total.width;
      if (handle.height > groo.height)
        handle.height = groo.height;
      if (handle.y < groo.y)
        handle.y = groo.y;
      if (handle.y+handle.height > groo.y+groo.height)
        handle.y = groo.y+groo.height-handle.height;
    }
  }

  inline void on_click(Widget* self, MouseButton);
  inline void on_hover(Widget* self);
  inline void on_leave(Widget* self);
  inline void on_drag(Widget* self);
  inline void on_release(Widget* self, MouseButton);
  inline void on_scroll(Widget* self);
  void update(Rect total);
  void draw(Rect total);
};

struct ScrollBar : Widget
{
  ScrollBarStyle style;

  ScrollBar(std::string&& id);

  inline ScrollBar* orient(Orientation new_orientation)
  {
    style.orient = new_orientation;
    return this;
  }

  void on_click(MouseButton) override;
  void on_hover() override;
  void on_leave() override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;

  void debug_draw() override;
  void update() override;
  void draw() override;
  ~ScrollBar() override;
};

inline void ScrollBarStyle::on_click(Widget* self, MouseButton btn)
{
  Rect total = self->rect;
  let mpo = GetMousePosition();
  if (CheckCollisionPointRec(mpo, min_btn(total)))
    min_btn_style.on_click(self, btn);
  else if (CheckCollisionPointRec(mpo, max_btn(total)))
    max_btn_style.on_click(self, btn);
  else if (CheckCollisionPointRec(mpo, handle))
    handle_style.on_click(self, btn);
  else
    /* TODO: move handle towards cursor and grab handle */;
  old_mpo = mpo;
}
inline void ScrollBarStyle::on_hover(Widget* self)
{
  Rect total = self->rect;
  let min_r = min_btn(self->rect);
  let max_r = max_btn(self->rect);
  let mpo = GetMousePosition();
  if (CheckCollisionPointRec(mpo, min_r))
    min_btn_style.on_hover();
  if (CheckCollisionPointRec(mpo, max_r))
    max_btn_style.on_hover();
  if (CheckCollisionPointRec(mpo, handle))
    handle_style.on_hover();

  let old_mpo = mpo - GetMouseDelta();
  let was_on = [mpo, old_mpo](Rect r){
    return (
        CheckCollisionPointRec(old_mpo, r)
    and not CheckCollisionPointRec(mpo, r)
    );
  };
  if (was_on(min_r))
    min_btn_style.on_leave();
  if (was_on(max_r))
    max_btn_style.on_leave();
  if (was_on(handle))
    handle_style.on_leave();
}
inline void ScrollBarStyle::on_leave(Widget* self)
{
  min_btn_style.on_leave();
  max_btn_style.on_leave();
  handle_style.on_leave();
}

inline void ScrollBarStyle::on_drag(Widget* self)
{
  // Note: we cannot use GetMouseDelta because on_drag could be called multiple times per update
  // in which case GetMouseDelta isn't reset between on_drag calls.
  let mpo = GetMousePosition();
  let delta = mpo - old_mpo;
  if (orient == HORIZONTAL)
    handle.x += delta.x;
  else
    handle.y += delta.y;
  old_mpo = mpo;
  clamp(self->rect);
  app().redraw();
}
inline void ScrollBarStyle::on_release(Widget* self, MouseButton btn)
{
  min_btn_style.on_release(
    self,
    [this, self]{
      value(self->rect, value(self->rect) - 0.1f);
      clamp(self->rect);
    },
    btn
  );
  max_btn_style.on_release(
    self,
    [this, self]{
      value(self->rect, value(self->rect) + 0.1f);
      clamp(self->rect);
    },
    btn
  );
  handle_style.on_release(
    self,
    nullptr,
    btn
  );
}

void ScrollBarStyle::on_scroll(Widget* w)
{
  value(w->rect, value(w->rect) - GetMouseWheelMove()*0.1f);
}


