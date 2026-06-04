#include "push_button.hpp"
#include "app.hpp"
#include "hull.hpp"
#include "raylib.h"
#include "text.hpp"
#include <string>

Color button_bg_style(ButtonState state)
{
  switch (state)
  {
    case BTN_RELEASED:
      return palette().bg(UI_INACTIVE);
    case BTN_HOVERED:
      return palette().bg(UI_FOCUSED);
    case BTN_DOWN:
      return palette().bg(UI_ACTIVE);
    case BTN_LOCKED:
      return palette().bg(UI_SELECTED);
  }
  return BLACK;
}
Color button_border_style(ButtonState state)
{
  switch (state)
  {
    case BTN_RELEASED:
      return palette().border(UI_DEFAULT);
    case BTN_HOVERED:
      return palette().border(UI_FOCUSED);
    case BTN_DOWN:
      return palette().border(UI_FOCUSED);
    case BTN_LOCKED:
      return palette().border(UI_FOCUSED);
  }
  return RED;
}

PushButton::PushButton(std::string&& id, std::function<void()>&& on_click)
  : Hull(std::move(id)),
    _on_click(std::move(on_click))
{}

PushButton* PushButton::set_text(std::string&& txt)
{
  add(
      (new Text(std::move(txt)))
      ->place(x(), y(), w(), h())
  );
  child->center_in_parent();
  fit_to_child();
  return this;
}

PushButton* PushButton::set_on_click(std::function<void()>&& on_click)
{
  _on_click = on_click;
  return this;
}

Widget* PushButton::request(std::string_view name)
{
  if (!child)
    return nullptr;
  if (child->_name == name)
    return child.get();
  return child->request(name);
}

void PushButton::on_click(MouseButton btn)
{ style.base.on_click(this, btn); }

void PushButton::on_hover()
{ style.base.on_hover(); }

void PushButton::on_leave()
{ style.base.on_leave(); }

void PushButton::on_release(MouseButton btn)
{
  style.base.on_release(
    this,
    _on_click,
    btn
  );
}

void PushButton::draw()
{
  if (!visible())
    return;
  macro i32 segments = 8;
  macro f32 thickness = 1.f;
  
  DrawRoundedBox(
    rect,
    style.roundness_left,
    style.roundness_right,
    thickness,
    style.border(),
    style.bg(),
    segments
  );
  if (style.update(GetFrameTime()))
    app().redraw();
  Hull::draw();
}

PushButton::~PushButton() {}

