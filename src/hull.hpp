#pragma once
#include "widget.hpp"

struct Hull : Widget
{
  uptr<Widget> child;
  Rect old_rect;
  f32 padding;

  Hull(std::string&& id);

  inline Hull* add(Own<Widget*> child)
  {
    assert(!this->child);
    this->child.reset(child);
    this->child->parent = this;
    return this;
  }
  inline Hull* fit_to_child(f32 padding = 5.f)
  { this->padding = padding; return this; }
  
  void update() override;
  ~Hull() override;
};
