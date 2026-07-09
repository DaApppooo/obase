#include "toggle_button.hpp"
#include "push_button.hpp"
#include "text.hpp"

ToggleButton::ToggleButton(std::string&& id, std::function<void(bool)>&& on_click)
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

ToggleButton* ToggleButton::set_on_click(std::function<void(bool)>&& on_click)
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
{ style.base.on_click(this, btn); }

void ToggleButton::on_hover()
{ style.base.on_hover(); }
void ToggleButton::on_leave()
{ style.base.on_leave(); }

void ToggleButton::on_release(MouseButton btn)
{
  style.base.on_release(
    this,
    [this](){
      if (_on_click)
        _on_click(style.state() == BTN_LOCKED);
      if (style.state() == BTN_DOWN)
        style.set_state(BTN_LOCKED);
      else
        style.set_state(BTN_HOVERED);
    },
    btn
  );
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
  style.update(GetFrameTime());
  Hull::draw();
}

ToggleButton::~ToggleButton() {}




