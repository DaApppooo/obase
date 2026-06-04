#include "scroll_bar.hpp"
#include "app.hpp"
#include "raylib.h"
#include "sys_settings.hpp"
#include "types.hpp"
#include "widget.hpp"

void ScrollBarStyle::update(const char* widget_name, Rect total)
{
  if (old_total_rect.width == 0 or old_total_rect.height == 0)
  {
    TraceLog(
      LOG_WARNING,
      "ScrollBar named '%s' doesn't have its size properly set !",
      widget_name
    );
    old_total_rect = total = {0, 0,
      BUTTON_MIN_SIZE*2.f+HANDLE_MIN_SIZE,
      BUTTON_MIN_SIZE*2.f+HANDLE_MIN_SIZE
    };
    handle = {
      BUTTON_MIN_SIZE, BUTTON_MIN_SIZE,
      HANDLE_MIN_SIZE, HANDLE_MIN_SIZE
    };
  }
  let rx = (handle.x - old_total_rect.x)/old_total_rect.width;
  let ry = (handle.y - old_total_rect.y)/old_total_rect.height;
  let rw = handle.width/old_total_rect.width;
  let rh = handle.height/old_total_rect.height;

  mut nx = rx * total.width + total.x;
  mut ny = ry * total.height + total.y;
  mut nw = rw * total.width;
  mut nh = rh * total.height;

  if (orient == HORIZONTAL)
    ny = total.y, nh = total.height;
  else
    nx = total.x, nw = total.width;

  handle = {nx, ny, nw, nh};
  
  old_total_rect = total;
}

void ScrollBarStyle::draw(Rect total)
{
  macro f32 roundness = 0.2f;
  macro f32 thickness = 1.f;
  let dt = GetFrameTime();
  if (min_btn_style.update(dt))
    app().redraw();
  if (max_btn_style.update(dt))
    app().redraw();
  if (handle_style.update(dt))
    app().redraw();
  DrawRoundedBox(
    groove(total),
    roundness, roundness,
    thickness,
    palette().border(UI_DEFAULT),
    palette().bg(UI_ACTIVE)
  );
  DrawRoundedBox(
    min_btn(total),
    roundness, roundness,
    thickness,
    min_btn_style.border(),
    min_btn_style.bg()
  );
  DrawRoundedBox(
    max_btn(total),
    roundness, roundness,
    thickness,
    max_btn_style.border(),
    max_btn_style.bg()
  );
  palette().draw_icon(
    ICON_ARROW_HEAD, reduce(min_btn(total), 3.f),
    orient == HORIZONTAL ? 180 : 90,
    min_btn_style.border()
  );
  palette().draw_icon(
    ICON_ARROW_HEAD, reduce(max_btn(total), 3.f),
    orient == HORIZONTAL ? 0 : -90,
    max_btn_style.border()
  );
  
  // handle
  DrawRoundedBox(
    handle,
    roundness, roundness,
    thickness,
    handle_style.border(),
    handle_style.bg()
  );
}

ScrollBar::ScrollBar(std::string&& id)
  : Widget(std::move(id))
{
  style.orient = VERTICAL;
}

void ScrollBar::on_click(MouseButton btn)
{ style.on_click(this, btn); }
void ScrollBar::on_drag(MouseButton btn)
{ style.on_drag(this); }
void ScrollBar::on_release(MouseButton btn)
{ style.on_release(this, btn); }
void ScrollBar::on_hover()
{ style.on_hover(this); }
void ScrollBar::on_leave()
{ style.on_leave(this); }

void ScrollBar::debug_draw()
{
  DrawRectangleLinesEx(style.min_btn(rect), 1.f, PURPLE);
  DrawRectangleLinesEx(style.max_btn(rect), 1.f, PINK);
  DrawRectangleLinesEx(style.handle, 1.f, DARKGREEN);
}
void ScrollBar::update()
{
  style.update(_name.c_str(), rect);
  Widget::update();
}
void ScrollBar::draw()
{ style.draw(rect); }
ScrollBar::~ScrollBar() {}
