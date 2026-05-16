#include "slider.hpp"
#include "app.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <format>
#include <ostream>

Slider::Slider(std::string&& name)
  : Widget(std::move(name)),
    _x(0.f), _anim(0.f),
    _auto_dents(0),
    _custom_dent(-1.f)
{}

void Slider::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  style.set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void Slider::on_scroll()
{
  let delta = GetMouseWheelMove();
  _x += delta/20.f;
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

void Slider::on_drag(MouseButton btn)
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

void Slider::on_hover()
{
  if (style.state == BTN_RELEASED)
  {
    app().redraw();
    style.set_state(BTN_HOVERED);
  }
}
void Slider::on_leave()
{
  if (style.state == BTN_HOVERED)
  {
    app().redraw();
    style.set_state(BTN_RELEASED);
  }
}

void Slider::on_release(MouseButton btn)
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

void Slider::help(std::ostream& out)
{
  std::print(out, "{} : {:.01}%", _name, _anim*100.f);
}

void Slider::update()
{
  Widget::update();
  if (isnanf(_anim))
    _anim = _x;
}

void Slider::draw()
{
  let angle = _anim*2.f*PI + PI/2.f;
  let color = style.border();
  DrawLineV(
    _pos(0),
    _pos(1),
    style.border()
  );
  if (_auto_dents)
  {
    for (int i = 0; i < _auto_dents; i++)
    {
      let index = i+1;
      let ratio = index / f32(_auto_dents+1);
      DrawLineV(
        _pos(ratio) + _orth()*_secondary_size()/4.f,
        _pos(ratio) + _orth()*_secondary_size()/2.f,
        palette().border(UI_ACTIVE)
      );
      DrawLineV(
        _pos(ratio) - _orth()*_secondary_size()/4.f,
        _pos(ratio) - _orth()*_secondary_size()/2.f,
        palette().border(UI_ACTIVE)
      );
    }
  }
  if (_custom_dent > 0.f)
  {
    let ratio = _custom_dent;
    DrawLineV(
      _pos(ratio) + _orth()*_secondary_size()/4.f,
      _pos(ratio) + _orth()*_secondary_size()/2.f,
      palette().border(UI_ACTIVE)
    );
    DrawLineV(
      _pos(ratio) - _orth()*_secondary_size()/4.f,
      _pos(ratio) - _orth()*_secondary_size()/2.f,
      palette().border(UI_ACTIVE)
    );
  }
  DrawCircleV(
    _handle_pos(),
    _handle_radius(),
    palette().border(UI_DEFAULT)
  );
  DrawLineV(
    _handle_pos() - _orth()*_secondary_size()/2.f,
    _handle_pos() + _orth()*_secondary_size()/2.f,
    color
  );
  if (style.update(GetFrameTime()))
    app().redraw();
}

Slider::~Slider() {}


