#include "toggle_icon.hpp"
#include "app.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "sys_settings.hpp"
#include "toggle_button.hpp"


ToggleIcon::ToggleIcon(std::string&& name, std::function<void(bool)>&& on_click)
  : ToggleButton(std::move(name), std::move(on_click)),
    off_icon(ICON_CLOSE), off_color({0}),
    on_icon(ICON_CLOSE), on_color({0}),
    hovering(0)
{}

void ToggleIcon::on_hover()
{
  ToggleButton::on_hover();
  hovering.set(1);
  hovering.update(GetFrameTime());
}

void ToggleIcon::on_leave()
{
  ToggleButton::on_leave();
  hovering.set(0);
  hovering.update(GetFrameTime());
}

void ToggleIcon::draw()
{
  if (!visible())
    return;
  assert(!child);
  let icon = is_on() ? on_icon : off_icon;
  Color color = off_color;
  if (style.base.state == BTN_LOCKED)
    color = style.base.fader.get(off_color, on_color);
  palette().draw_icon(
    icon,
    reduce(rect, -hovering.get() * 3),
    0,
    color
  );
  let dt = GetFrameTime();
  style.update(dt*0.5f);
  hovering.update(dt);
}

