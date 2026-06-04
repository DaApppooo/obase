#pragma once
#include "raylib.h"
#include "scroll_bar.hpp"
#include "widget.hpp"

struct ResizableScrollBar : ScrollBar
{
  static macro f32 GRIPS_SIZE = 10.f;
  enum Gripped
  {
    NONE, MIN, MAX
  } gripped;
  
  ResizableScrollBar(std::string&& id);

  inline Rect min_grip() const
  {
    if (style.orient == HORIZONTAL)
      return {
        style.handle.x, style.handle.y,
        GRIPS_SIZE, style.handle.height
      };
    else
      return {
        style.handle.x, style.handle.y + style.handle.height - GRIPS_SIZE,
        style.handle.width, GRIPS_SIZE
      };
  }

  inline Rect max_grip() const
  {
    if (style.orient == HORIZONTAL)
      return {
        style.handle.x + style.handle.width - GRIPS_SIZE, style.handle.y,
        GRIPS_SIZE, style.handle.height
      };
    else
      return {
        style.handle.x, style.handle.y,
        style.handle.width, GRIPS_SIZE
      };
  }

  void on_click(MouseButton) override;
  void on_drag(MouseButton) override;
  void on_hover() override;
  void on_leave() override;
  void on_release(MouseButton) override;
};
