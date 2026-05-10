#include "layout.hpp"
#include "raylib.h"
#include "widget.hpp"

Layout::Layout(std::string&& name)
  : Widget(std::move(name)),
    children(),
    gap(0)
{}

void Layout::debug_draw()
{
  for (uptr<Widget>& w : children)
    w->debug_draw();
  Widget::debug_draw();
}

void Layout::draw()
{
  for (uptr<Widget>& w : children)
    w->draw();
}

void Layout::update()
{
  if (parent == nullptr)
  {
    x(0);
    y(0);
    w(GetScreenWidth());
    h(GetScreenHeight());
  }
  for (uptr<Widget>& child : children)
  {
    let rx = (child->rect.x - old_rect.x)/old_rect.width;
    let ry = (child->rect.y - old_rect.y)/old_rect.height;
    let rw = child->rect.width/old_rect.width;
    let rh = child->rect.height/old_rect.height;

    let nx = rx * rect.width + rect.x;
    let ny = ry * rect.height + rect.y;
    let nw = rw * rect.width;
    let nh = rh * rect.height;

    child->rect = { nx, ny, nw, nh };
    child->update();
  }
  old_rect = rect;
}

Layout::~Layout()
{}
