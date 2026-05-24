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

PushButton::PushButton(std::string&& id, std::function<void(MouseButton)>&& on_click)
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

PushButton* PushButton::set_on_click(std::function<void(MouseButton)>&& on_click)
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
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (app().focused)
    return;
  style.set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void PushButton::on_hover()
{
  if (style.state() == BTN_RELEASED)
  {
    app().redraw();
    style.set_state(BTN_HOVERED);
  }
}
void PushButton::on_leave()
{
  if (style.state() == BTN_HOVERED)
  {
    app().redraw();
    style.set_state(BTN_RELEASED);
  }
}

void PushButton::on_release(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state() != BTN_DOWN)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), rect))
  {
    if (_on_click)
      _on_click(btn);
    style.set_state(BTN_HOVERED);
  }
  else
    style.set_state(BTN_RELEASED);
  app().redraw();
  UNFOCUS_ME;
}

void PushButton::draw()
{
  if (!visible())
    return;
  macro i32 segments = 8;
  macro f32 thickness = 1.f;
  
  DrawRectangleRoundedPro(
    reduce(rect, -thickness),
    style.roundness_left,
    style.roundness_right,
    segments,
    style.border()
  );
  DrawRectangleRoundedPro(
    rect,
    style.roundness_left,
    style.roundness_right,
    segments,
    style.bg()
  );
  if (style.update(GetFrameTime()))
    app().redraw();
  Hull::draw();
}

PushButton::~PushButton() {}

