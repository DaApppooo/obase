#include "layout.hpp"
#include "app.hpp"
#include "raylib.h"
#include "widget.hpp"

Layout::Layout(std::string&& name)
  : Widget(std::move(name)),
    children(),
    gap(0)
{}

Widget* Layout::request(std::string_view name)
{
  Widget* ptr;
  for (uptr<Widget>& w : children)
  {
    if (w->_name == name)
      return w.get();
    if ((ptr = w->request(name)))
      return ptr;
  }
  return nullptr;
}

void Layout::debug_draw()
{
  for (uptr<Widget>& w : children)
    w->debug_draw();
  Widget::debug_draw();
}

void Layout::draw()
{
  let old_scissor = app().scissor_begin(rect);
  for (uptr<Widget>& w : children)
    w->draw();
  app().scissor_end(old_scissor);
}

void rescale_child(Widget &child, Rect old_rect, Rect rect)
{
  let rx = (child.rect.x - old_rect.x)/old_rect.width;
  let ry = (child.rect.y - old_rect.y)/old_rect.height;
  let rw = child.rect.width/old_rect.width;
  let rh = child.rect.height/old_rect.height;

  let nx = rx * rect.width + rect.x;
  let ny = ry * rect.height + rect.y;
  mut nw = rw * rect.width;
  mut nh = rh * rect.height;

  if (child.flags & W_LOCKED)
    nw = child.rect.width;
  if (child.flags & H_LOCKED)
    nh = child.rect.height;

  child.rect = { nx, ny, nw, nh };
}

void Layout::update()
{
  Widget::update();
  if (parent == nullptr)
  {
    x(0);
    y(0);
    w(GetScreenWidth());
    h(GetScreenHeight());
  }
  if (old_rect.width == 0 || old_rect.height == 0)
    old_rect = rect;
  for (uptr<Widget>& child : children)
  {
    rescale_child(*child, old_rect, rect);
    child->update();
  }
  old_rect = rect;
}

Layout::~Layout()
{}

#define PASS_ON_MOUSE(METH, BTN_T, NAME) \
void Layout::METH(BTN_T NAME) \
{ \
  for (uptr<Widget>& w : children) \
  { \
    if (CheckCollisionPointRec(GetMousePosition(), w->rect)) \
      w->METH(NAME); \
  } \
}

PASS_ON_MOUSE(on_hover,,);
PASS_ON_MOUSE(on_scroll,,);
PASS_ON_MOUSE(on_double_click,,);
PASS_ON_MOUSE(on_click, MouseButton, btn);
PASS_ON_MOUSE(on_drag, MouseButton, btn);
PASS_ON_MOUSE(on_release, MouseButton, btn);

