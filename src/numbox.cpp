#include "numbox.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "widget.hpp"
#include <cmath>

NumBox::NumBox(std::string&& id)
  : Widget(std::move(id)),
    _x(0.f),
    min(0.f), max(1.f),
    style(),
    pow_min(-3), pow_max(0),
    _pow_sel(1)
 {}

void NumBox::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  style.set_state(BTN_DOWN);
  app().redraw();
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
  
  let rel_mpo = GetMousePosition().x - x();
  if (rel_mpo <= padding or rel_mpo >= w()-padding)
    return;
  _pow_sel = digit_power(rel_mpo);
}

void NumBox::on_scroll()
{
  let delta = GetMouseWheelMove();
  let rel_mpo = GetMousePosition().x - x();
  if (rel_mpo <= padding or rel_mpo >= w()-padding)
    return;
  i32 power = digit_power(rel_mpo);
  _x += delta*std::pow(10.0, power);
  _x = std::clamp(_x, min, max);
  app().redraw();
}

void NumBox::on_drag(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state != BTN_DOWN)
    return;
  let delta = GetMouseDelta();
  _x += (delta.x-delta.y)/50.0*std::pow(10.0, _pow_sel);
  _x = std::clamp(_x, min, max);
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
  _pow_sel = pow_max+1;
  app().redraw();
  UNFOCUS_ME;
}

void NumBox::update()
{
  Widget::update();
  if (flags & W_LOCKED)
    assert(!"Numboxes must not have their width locked. Use ->unlock_width() after ->size().");
  let& pal = palette();
  let digit_size = _font_size()/pal.text_size*pal.largest_digit_w;
  let dot_size = _font_size()/pal.text_size*pal.dot_w;
  let digit_count = pow_max - std::min(pow_min, 0) + 1;
  let new_w = digit_count*digit_size+dot_size + padding*2.f;
  w(new_w);
  if (new_w != w())
    app().redraw();
}

void NumBox::help(std::ostream& out)
{
  std::print(out, "{} : {:.01}%", _name, _x*100.f);
}

void NumBox::draw()
{
  static std::string buffer;

  let& pal = palette();
  let digit_size = _font_size()/pal.text_size*pal.largest_digit_w;
  let dot_size = _font_size()/pal.text_size*pal.dot_w;
  macro i32 segments = 8;
  macro f32 thickness = 1.f;
  macro f32 roundness = 0.1f;
  buffer.resize(64);
  let width = pow_max - pow_min + 2;
  std::format_to_n(buffer.begin(), 64, "{:{}.{}f}", _x, width,  -pow_min);
  
  DrawRectangleRoundedPro(
    reduce(rect, -thickness),
    roundness,
    roundness,
    segments,
    style.border()
  );
  DrawRectangleRoundedPro(
    rect,
    roundness,
    roundness,
    segments,
    style.bg()
  );
  mut cursor = pow_max+1;
  let mpo = GetMousePosition();
  if (style.state == BTN_DOWN)
    cursor = _pow_sel;
  else
    cursor = digit_power(mpo.x-x());
  if (cursor <= pow_max && CheckCollisionPointRec(mpo, rect))
  {
    DrawRectangleRoundedPro(
      { x()+digit_pos(cursor)+padding, y()+padding, digit_size, _font_size() },
      roundness*2.f,
      roundness*2.f,
      segments,
      ColorAlpha(style.border(), 0.2f)
    );
  }
  f32 ix = x()+padding;
  char str[2] = {0, 0};
  let real_pow_min = std::min(pow_min, 0);
  for (i32 pow = pow_max; pow >= real_pow_min; pow--)
  {
    let index = (pow_max - real_pow_min) - (pow - real_pow_min) + (pow < 0);
    str[0] = buffer[index];
    DrawTextEx(font(), str, {ix, y()+padding}, _font_size(), 2.f, pal.text);
    if (pow == 0)
    {
      DrawTextEx(
        font(), ".", {ix+digit_size, y()+padding}, _font_size(), 2.f, pal.text
      );
      ix += dot_size;
    }
    ix += digit_size;
  }
  if (style.update(GetFrameTime()))
    app().redraw();
}

 


