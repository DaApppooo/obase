#include "toggle_button.hpp"
#include "layout.hpp"
#include "push_button.hpp"
#include "text.hpp"

ToggleButton::ToggleButton(std::string&& id, std::function<void(MouseButton, bool)>&& on_click)
  : Hull(std::move(id)),
    _on_click(std::move(on_click)),
    old_state(BTN_RELEASED),
    state(BTN_RELEASED),
    fader()
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
  if (state == BTN_LOCKED)
    return;
  set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void ToggleButton::on_hover()
{
  if (state == BTN_RELEASED)
  {
    app().redraw();
    set_state(BTN_HOVERED);
  }
}
void ToggleButton::on_leave()
{
  if (state == BTN_HOVERED)
  {
    app().redraw();
    set_state(BTN_RELEASED);
  }
}

void ToggleButton::on_release(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (state != BTN_DOWN && state != BTN_LOCKED)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), rect))
  {
    _on_click(btn, state == BTN_LOCKED);
    if (state == BTN_DOWN)
      set_state(BTN_LOCKED);
    else
      set_state(BTN_HOVERED);
  }
  else
    set_state(BTN_RELEASED);
  app().redraw();
  UNFOCUS_ME;
}

void ToggleButton::update()
{
  if (child)
  {
    rescale_child(*child, old_rect, rect);
    child->update();
    fit_child(*this, *child, 5.f);
  }
  Widget::update();
  old_rect = rect;
}

void ToggleButton::debug_draw()
{
  Widget::debug_draw();
  if (child)
    child->debug_draw();
}

void ToggleButton::draw()
{
  macro i32 segments = 8;
  macro f32 thickness = 1.f;
  
  DrawRectangleRoundedPro(
    reduce(rect, -thickness),
    style.roundness_left,
    style.roundness_right,
    segments,
    fader.get(button_border_style(old_state), button_border_style(state))
  );
  DrawRectangleRoundedPro(
    rect,
    style.roundness_left,
    style.roundness_right,
    segments,
    fader.get(button_bg_style(old_state), button_bg_style(state))
  );
  if (fader.update(GetFrameTime()))
    app().redraw();
  assert(child);
  if (child)
    child->draw();
}

ToggleButton::~ToggleButton() {}




