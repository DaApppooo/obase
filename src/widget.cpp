#include "widget.hpp"
#include <cmath>
#include <cstddef>
#include "macros.hpp"
#include "raylib.h"
#include "types.hpp"
#include "app.hpp"

Widget::Widget(std::string&& name)
  : anchors{Anchor(), Anchor(), Anchor(), Anchor()},
    parent(nullptr),
    _name(std::move(name)),
    rect(0, 0, 0, 0),
    flags(0)
{}

void Widget::debug_draw()
{
  let hue = fmodf(ptrdiff_t(parent), 360.f);
  let color = ColorFromHSV(hue, 1.f, 1.f);
  DrawRectangleRec(rect, color);
#ifdef _DEBUG
  DrawText(_name.c_str(), rect.x, rect.y + 2.f, 30, color);
#endif
}

Widget* Widget::request(std::string_view name)
{ return nullptr; }

void Widget::on_click(MouseButton) {}
void Widget::on_hover() {}
void Widget::on_scroll() {}
void Widget::on_double_click() {}
void Widget::on_drag(MouseButton) {}
void Widget::on_release(MouseButton) {}
void Widget::on_keydown(KeyboardKey) {}
void Widget::on_keyup(KeyboardKey) {}

void Widget::update()
{
  for (mut anch : anchors)
  {
    if (anch.update())
      app().redraw();
  }
}

Widget::~Widget() {}

