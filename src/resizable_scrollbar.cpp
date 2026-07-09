#include "resizable_scrollbar.hpp"
#include "raylib.h"
#include "scroll_bar.hpp"
#include "types.hpp"
#include "widget.hpp"


ResizableScrollBar::ResizableScrollBar(std::string&& id)
  : ScrollBar(std::move(id)),
    gripped(NONE)
{}

void ResizableScrollBar::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  let mpo = GetMousePosition();
  let handle = style.handle;
  
  if (CheckCollisionPointRec(mpo, min_grip()))
    gripped = MIN;
  else if (CheckCollisionPointRec(mpo, max_grip()))
    gripped = MAX;
  else
    style.on_click(this, btn);
  if (gripped != NONE)
    FOCUS_ME;
}

void ResizableScrollBar::on_hover()
{
  let min_rect = min_grip();
  let max_rect = max_grip();
  let mpo = GetMousePosition();

  if (
     CheckCollisionPointRec(mpo, min_rect)
  or CheckCollisionPointRec(mpo, max_rect)
  ) {
    app().set_cursor(
      style.orient == VERTICAL
      ? MOUSE_CURSOR_RESIZE_NS
      : MOUSE_CURSOR_RESIZE_EW // ewwwww
    );
  }
  else
  {
    app().set_cursor(MOUSE_CURSOR_ARROW);
    style.on_hover(this);
  }
}

void ResizableScrollBar::on_leave()
{
  app().set_cursor(MOUSE_CURSOR_ARROW);
  style.on_leave(this);
}


void ResizableScrollBar::on_drag(MouseButton btn)
{
  mut mpo = GetMousePosition();
  let groove = style.groove(rect);
  let combine = [](Gripped grip, Orientation orient) constexpr
  { return (static_cast<u8>(grip) << 4) | static_cast<u8>(orient); };
  
  mpo.x = std::clamp(mpo.x, groove.x, groove.x+groove.width);
  mpo.y = std::clamp(mpo.y, groove.y, groove.y+groove.height);
  if (gripped != NONE)
  {
    switch (combine(gripped, style.orient))
    {
      case combine(MAX, HORIZONTAL):
        style.handle.width = std::max(
          mpo.x - style.handle.x,
          ScrollBarStyle::HANDLE_MIN_SIZE
        );
        break;
      case combine(MIN, HORIZONTAL):
        style.handle.width -= mpo.x - style.handle.x;
        if (style.handle.width < ScrollBarStyle::HANDLE_MIN_SIZE)
          style.handle.width = ScrollBarStyle::HANDLE_MIN_SIZE;
        else
          style.handle.x = mpo.x;
        break;
      case combine(MIN, VERTICAL):
        style.handle.height = std::max(
          mpo.y - style.handle.y,
          ScrollBarStyle::HANDLE_MIN_SIZE
        );
        break;
      case combine(MAX, VERTICAL):
        style.handle.height -= mpo.y - style.handle.y;
        if (style.handle.height < ScrollBarStyle::HANDLE_MIN_SIZE)
          style.handle.height = ScrollBarStyle::HANDLE_MIN_SIZE;
        else
          style.handle.y = mpo.y;
        break;
    }
  }
  else
    style.on_drag(this);
}


void ResizableScrollBar::on_release(MouseButton btn)
{
  gripped = NONE;
  style.on_release(this, btn, true);
}
