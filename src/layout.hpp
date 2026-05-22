#pragma once
#include "widget.hpp"
#include <concepts>
#include <string_view>
#include <vector>

struct Layout : Widget
{
  std::vector<uptr<Widget>> children;
  Rect old_rect;
  f32 padding;

  Layout(std::string&& name);

  template <class... Elements>
    requires (std::convertible_to<Elements*, Widget*> && ...)
  Layout* add(Own<Elements*>... elements);

  virtual Layout* insert(size_t index, Own<Widget*> elem);
  virtual Layout* remove(size_t index);
  virtual Layout* remove(std::string_view name);

  inline Layout* fit_to_child(f32 padding = 5.f)
  { this->padding = padding; return this; }
  Widget* request(std::string_view name) override;

  template <class Func>
  inline Layout* map(Func&& f)
  {
    for (mut& child : children)
      f(child.get());
    return this;
  }
  template <class Func, class Buffer>
  inline Layout* fetch_into(Func&& f, Buffer&& buffer)
  {
    for (mut& child : children)
      buffer << f(child.get());
    return this;
  }
    
  void on_hover() override;
  void on_click(MouseButton) override;
  void on_double_click() override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_scroll() override;
  
  void debug_draw() override;
  void draw() override;
  void update() override;
  ~Layout() override;
};

template <class... Elements>
  requires (std::convertible_to<Elements*, Widget*> && ...)
Layout* Layout::add(Own<Elements*>... elements)
{
  let start = children.size();
  children.reserve(children.capacity() + sizeof...(elements));
  (children.push_back(uptr<Widget>(elements)), ...);
  ((elements->parent = this), ...);
  return this;
}

void fit_child(Widget& self, Widget& child, f32 margin);
void fit_children(Widget& self, std::span<uptr<Widget>> children, f32 margin);
void rescale_child(Widget& child, Rect old_rect, Rect rect);


