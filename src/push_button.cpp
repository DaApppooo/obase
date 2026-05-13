#include "push_button.hpp"
#include "app.hpp"
#include "layout.hpp"
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
  }
  return BLACK;
}
Color button_border_style(ButtonState state)
{
  switch (state)
  {
    case BTN_RELEASED:
      return palette().bg(UI_INACTIVE);
    case BTN_HOVERED:
      return palette().bg(UI_FOCUSED);
    case BTN_DOWN:
      return palette().bg(UI_ACTIVE);
  }
  return RED;
}

PushButton::PushButton(std::string&& id, std::function<void(MouseButton)>&& on_click)
  : Widget(std::move(id)),
    _on_click(std::move(on_click)),
    child(nullptr),
    old_state(BTN_RELEASED),
    state(BTN_RELEASED),
    fader()
{}

PushButton* PushButton::set_text(std::string&& txt)
{
  add(new Text(std::move(txt)));
  child->fill_parent();
  return this;
}

PushButton* PushButton::set_on_click(std::function<void(MouseButton)>&& on_click)
{
  _on_click = on_click;
  return this;
}

PushButton* PushButton::add(Own<Widget*> child)
{
  assert(!this->child);
  this->child.reset(child);
  child->parent = this;
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
  state = BTN_DOWN;
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void PushButton::on_hover()
{
  if (state == BTN_RELEASED)
  {
    app().redraw();
    state = BTN_HOVERED;
  }
}

void PushButton::on_release(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (state != BTN_DOWN)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), rect))
    _on_click(btn);
  old_state = state;
  state = BTN_RELEASED;
  app().redraw();
  UNFOCUS_ME;
}

void PushButton::update()
{
  if (child)
  {
    rescale_child(*child, old_rect, rect);
    child->update();
  }
  Widget::update();
  old_rect = rect;
}

void PushButton::draw()
{
  macro f32 roundness = 0.01f;
  macro i32 segments = 8;
  macro f32 thickness = 1.f;
  
  DrawRectangleRounded(
    rect,
    roundness,
    segments,
    fader.get(button_bg_style(old_state), button_bg_style(state))
  );
  DrawRectangleRoundedLinesEx(
    rect,
    roundness,
    segments,
    thickness,
    fader.get(button_border_style(old_state), button_border_style(state))
  );
  if (fader.update(GetFrameTime()))
    app().redraw();
  if (child)
    child->draw();
}

PushButton::~PushButton() {}

