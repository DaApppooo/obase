#include "hull.hpp"
#include "layout.hpp"
#include "widget.hpp"
#include <string_view>
#include "raymath.h"

Hull::Hull(std::string&& id)
  : Widget(std::move(id)),
    child(nullptr),
    old_rect(0, 0, 1, 1),
    padding(-1)
{}

void Hull::debug_draw()
{
  Widget::debug_draw();
  if (child)
    child->debug_draw();
}

Widget* Hull::request(std::string_view name)
{
  if (child->_name == name)
    return child.get();
  return child->request(name);
}

void Hull::draw()
{
  if (!visible())
    return;
  if (child)
    child->draw();
}

void Hull::update()
{
  if (padding > 0 && (flags & W_LOCKED or flags & H_LOCKED))
    assert(!"Hull must both fit child and respect a fixed size, how ?");
  if (child)
  {
    rescale_child(*child, old_rect, rect);
    child->update();
    if (padding > 0)
      fit_child(*this, *child, padding);
  }
  Widget::update();
  old_rect = rect;
}

#define PASS_ON_MOUSE(METH, BTN_T, NAME) \
void Hull::METH(BTN_T NAME) \
{ \
  if (!_visible) \
    return; \
  if (CheckCollisionPointRec(GetMousePosition(), child->rect)) \
    child->METH(NAME); \
}

void Hull::on_leave()
{
  if (!_visible)
    return;
  let mpo = GetMousePosition();
  let old_mpo = mpo - GetMouseDelta();
  if (CheckCollisionPointRec(old_mpo, child->rect))
    child->on_leave();
}

void Hull::on_hover()
{
  if (!_visible)
    return;
  let mpo = GetMousePosition();
  let old_mpo = mpo - GetMouseDelta();
  if (CheckCollisionPointRec(mpo, child->rect))
    child->on_hover();
  else if (
     CheckCollisionPointRec(old_mpo, child->rect)
  )
    child->on_leave();
}
PASS_ON_MOUSE(on_scroll,,);
PASS_ON_MOUSE(on_double_click,,);
PASS_ON_MOUSE(on_click, MouseButton, btn);
PASS_ON_MOUSE(on_drag, MouseButton, btn);
PASS_ON_MOUSE(on_release, MouseButton, btn);

Hull::~Hull() {}
