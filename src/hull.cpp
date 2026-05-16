#include "hull.hpp"
#include "layout.hpp"
#include "widget.hpp"

Hull::Hull(std::string&& id)
  : Widget(std::move(id)),
    child(nullptr),
    old_rect(0, 0, 1, 1),
    padding(-1)
{}

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

Hull::~Hull() {}
