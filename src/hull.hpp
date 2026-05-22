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
  inline Hull* size(Vec2 size)
  { w(size.x); h(size.y); flags |= W_LOCKED | H_LOCKED; padding = -1.f; return this; }
  inline Hull* size(f32 w, f32 h) { size({w, h}); return this; }
  inline Hull* size(f32 side) { size({side, side}); return this; }

  template <class Func>
  inline Hull* map(Func&& f)
  {
    f(child.get());
    return this;
  }
  template <class Func>
  inline auto fetch(Func&& f)
  {
    return f(child.get());
  }
  
  Widget* request(std::string_view name) override;
  void draw() override;
  void debug_draw() override;
  void update() override;
  ~Hull() override;
};
