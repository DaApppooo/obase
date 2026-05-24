#include "toggle_button.hpp"
#include "push_button.hpp"
#include "text.hpp"

ToggleButton::ToggleButton(std::string&& id, std::function<void(MouseButton, bool)>&& on_click)
  : Hull(std::move(id)),
    _on_click(std::move(on_click))
{}

ToggleButton* ToggleButton::set_text(std::string&& txt)
{
  add(
      (new Text(std::move(txt)))
      ->place(x(), y(), w(), h())
  );
  child->center_in_parent();
  fit_to_child();
  return this;
}

ToggleButton* ToggleButton::set_on_click(std::function<void(MouseButton, bool)>&& on_click)
{
  _on_click = std::move(on_click);
  return this;
}

ToggleButton* ToggleButton::add(Own<Widget*> child)
{
  assert(!this->child);
  this->child.reset(child);
  child->parent = this;
  return this;
}

Widget* ToggleButton::request(std::string_view name)
{
  if (!child)
    return nullptr;
  if (child->_name == name)
    return child.get();
  return child->request(name);
}

void ToggleButton::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state() == BTN_LOCKED)
    return;
  if (app().focused)
    return;
  style.set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void ToggleButton::on_hover()
{
  if (style.state() == BTN_RELEASED)
  {
    app().redraw();
    style.set_state(BTN_HOVERED);
  }
}
void ToggleButton::on_leave()
{
  if (style.state() == BTN_HOVERED)
  {
    app().redraw();
    style.set_state(BTN_RELEASED);
  }
}

void ToggleButton::on_release(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state() != BTN_DOWN && style.state() != BTN_LOCKED)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), rect))
  {
    if (_on_click)
      _on_click(btn, style.state() == BTN_LOCKED);
    if (style.state() == BTN_DOWN)
      style.set_state(BTN_LOCKED);
    else
      style.set_state(BTN_HOVERED);
  }
  else
    style.set_state(BTN_RELEASED);
  app().redraw();
  UNFOCUS_ME;
}

void ToggleButton::draw()
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

ToggleButton::~ToggleButton() {}




