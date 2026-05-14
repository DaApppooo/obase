#include "layout.hpp"
#include "app.hpp"
#include "raylib.h"
#include "widget.hpp"
#include "raymath.h"

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

void fit_child(Widget &self, Widget &child, f32 margin)
{
  self.rect = {
    child.rect.x - margin,
    child.rect.y - margin,
    child.rect.width + margin*2.f,
    child.rect.height + margin*2.f
  };
}

void fit_children(Widget &self, std::span<uptr<Widget>> children, f32 margin)
{
  Rectangle fit = self.rect;
  for (uptr<Widget>& w: children)
  {
    if (isnanf(fit.x))
      fit.x = w->x();
    else if (w->x() < fit.x)
    {
      fit.width += w->x() - fit.x;
      fit.x = w->x();
    }
    if (isnanf(fit.y))
      fit.y = w->y();
    else if (w->y() < fit.y)
    {
      fit.height += w->y() - fit.y;
      fit.y = w->y();
    }
    if (isnanf(fit.width))
      fit.width = w->w();
    else if (w->right() > fit.x+fit.width)
    {
      fit.width += w->right() - fit.x - fit.width;
    }
    if (isnanf(fit.height))
      fit.height = w->h();
    else if (w->right() > fit.y+fit.height)
    {
      fit.height += w->right() - fit.y - fit.height;
    }
  }
  fit.x -= margin;
  fit.y -= margin;
  fit.width += margin*2.f;
  fit.height += margin*2.f;
  self.rect = fit;
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
  fit_children(*this, children, padding);
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

void Layout::on_hover()
{
  let mpo = GetMousePosition();
  let old_mpo = mpo - GetMouseDelta();
  for (uptr<Widget>& w : children)
  {
    if (CheckCollisionPointRec(mpo, w->rect))
      w->on_hover();
    else if (CheckCollisionPointRec(old_mpo, w->rect))
      w->on_leave();
  }
}
PASS_ON_MOUSE(on_scroll,,);
PASS_ON_MOUSE(on_double_click,,);
PASS_ON_MOUSE(on_click, MouseButton, btn);
PASS_ON_MOUSE(on_drag, MouseButton, btn);
PASS_ON_MOUSE(on_release, MouseButton, btn);
