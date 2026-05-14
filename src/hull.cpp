#include "hull.hpp"
#include "layout.hpp"

Hull::Hull(std::string&& id)
  : Widget(std::move(id)),
    child(nullptr),
    old_rect(0, 0, 1, 1),
    padding(-1)
{}



void Hull::update()
{
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
