#include "knob.hpp"
#include "app.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <format>
#include <ostream>

Knob::Knob(std::string&& name)
  : Widget(std::move(name)),
    _x(0.f), _anim(0.f),
    _auto_dents(0),
    _custom_dent(-1.f)
{}

void Knob::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  style.set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void Knob::on_scroll()
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

void Knob::on_drag(MouseButton btn)
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

void Knob::on_hover()
{
  if (style.state == BTN_RELEASED)
  {
    app().redraw();
    style.set_state(BTN_HOVERED);
  }
}
void Knob::on_leave()
{
  if (style.state == BTN_HOVERED)
  {
    app().redraw();
    style.set_state(BTN_RELEASED);
  }
}

void Knob::on_release(MouseButton btn)
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

void Knob::help(std::ostream& out)
{
  std::print(out, "{} : {:.01}%", _name, _anim*100.f);
}

void Knob::update()
{
  Widget::update();
  if (isnanf(_anim))
    _anim = _x;
  if (w() < 10.f)
    w(10.f);
  if (h() < 10.f)
    h(10.f);
}

void Knob::draw()
{
  let center = Vec2{rect.x+rect.width/2.f, rect.y+rect.height/2.f};
  let M_radius = std::min(rect.width/2.f, rect.height/2.f);
  let radius = M_radius - 1.f;
  let angle = _anim*2.f*PI + PI/2.f;
  let color = style.border();
  DrawCircleSector(
    center,
    M_radius,
    PI/2.f*RAD2DEG, angle*RAD2DEG,
    64,
    color
  );
  DrawCircleV(
    center,
    radius,
    style.bg()
  );
  if (_auto_dents)
  {
    for (int i = 0; i < _auto_dents; i++)
    {
      let index = i+1;
      let ratio = index / f32(_auto_dents+1);
      let angle = ratio*2.f*PI + PI/2.f;
      DrawCircleV(
        center + Vec2{cosf(angle), sinf(angle)} * (radius - 5.f),
        2.5f,
        palette().border(UI_INACTIVE)
      );
    }
  }
  if (_custom_dent > 0.f)
  {
    let angle = _custom_dent*2.f*PI + PI/2.f;
    DrawCircleV(
      center + Vec2{cosf(angle), sinf(angle)} * (radius - 5.f),
      2.5f,
      palette().border(UI_INACTIVE)
    );
  }
  DrawCircleV(
    center + Vec2{cosf(PI/2.f), sinf(PI/2.f)} * (radius - 5.f),
    2.5f,
    palette().border(UI_INACTIVE)
  );
  DrawCircleV(
    center + Vec2{cosf(angle), sinf(angle)} * (radius - 5.f),
    2.5f,
    color
  );
  if (style.update(GetFrameTime()))
    app().redraw();
}

Knob::~Knob() {}

