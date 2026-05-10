#pragma once
#include "widget.hpp"
#include <concepts>
#include <vector>

struct Layout : Widget
{
  std::vector<uptr<Widget>> children;
  Rect old_rect;
  f32 gap; // gap between children

  Layout(std::string&& name);

  template <class... Elements>
    requires (std::convertible_to<Elements*, Widget*> && ...)
  Layout* add(Orientation, Own<Elements*>... elements);

  inline Layout* set_gap(f32 x) { gap = x; return this; }
  
  void on_hover() override;
  void on_click(MouseButton) override;
  void on_double_click() override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_scroll() override;
  void on_keydown(KeyboardKey key) override;
  void on_keyup(KeyboardKey key) override;
  
  void debug_draw() override;
  void draw() override;
  void update() override;
  ~Layout() override;
};

template <class... Elements>
  requires (std::convertible_to<Elements*, Widget*> && ...)
Layout* Layout::add(Orientation orient, Own<Elements*>... elements)
{
  let start = children.size();
  children.reserve(children.capacity() + sizeof...(elements));
  (children.push_back(uptr<Widget>(elements)), ...);
  ((elements->parent = this), ...);
  f32 pos = 0.f;
  for (size_t i = start; i < children.size(); i++)
  {
    uptr<Widget>& w = children[i];
    if (orient == HORIZONTAL)
    {
      
    }
    else
    {
      
    }
  }
  return this;
}

