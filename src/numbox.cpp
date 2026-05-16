#include "numbox.hpp"
#include "raylib.h"

NumBox::NumBox(std::string&& id)
  : Widget(std::move(id)),
    _x(0.f), _anim(0.f),
    min(0.f), max(1.f),
    style(),
    pow_min(-3), pow_max(0)
 {}

void NumBox::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  style.set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void NumBox::on_scroll()
{
  let delta = GetMouseWheelMove();
  let rel_mpo = GetMousePosition().x - x();
  
  app().redraw();
}

void NumBox::on_drag(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state != BTN_DOWN)
    return;
  let delta = GetMouseDelta();
  _x += (delta.x-delta.y)/500.f;
  _x = std::clamp(_x, 0.f, 1.f);
  _anim = _x;
  if (_auto_dents)
  {
    _anim = floorf(_x*(_auto_dents+1))/(_auto_dents+1);
  }
  if (_custom_dent > 0.f)
  {
    if (fabsf(_x - _custom_dent) < 0.05f)
      _anim = _custom_dent;
  }
  app().redraw();
}

void NumBox::on_hover()
{
  if (style.state == BTN_RELEASED)
  {
    app().redraw();
    style.set_state(BTN_HOVERED);
  }
}
void NumBox::on_leave()
{
  if (style.state == BTN_HOVERED)
  {
    app().redraw();
    style.set_state(BTN_RELEASED);
  }
}

void NumBox::on_release(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state != BTN_DOWN)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), rect))
  {
    style.set_state(BTN_HOVERED);
  }
  else
    style.set_state(BTN_RELEASED);
  app().redraw();
  UNFOCUS_ME;
}

void NumBox::help(std::ostream& out)
{
  std::print(out, "{} : {:.01}%", _name, _x*100.f);
}

 


