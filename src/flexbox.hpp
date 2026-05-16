#pragma once
#include "anchor.hpp"
#include "layout.hpp"
#include "widget.hpp"
#include <concepts>

enum Direction
{
  L2R, R2L,
  T2B, B2T
};

struct FlexBox : Layout
{
  f32 margin;
  Direction dir;
  
  FlexBox(std::string&& id);

  inline FlexBox* set_direction(Direction d)
  { dir = d; return this; }

  inline Orientation orientation() const
  { return dir == L2R || dir == R2L ? HORIZONTAL : VERTICAL; }

  void on_leave() override;
  void update() override;

  // flexbox automatically fits to children using anchors (no fit_children algorithm)
  inline Layout* fit_to_child(f32 padding = 5.f) = delete;

  FlexBox* set_padding(f32 pad);
  FlexBox* set_margin(f32 marge);

  template <class... Elements>
    requires (std::convertible_to<Elements*, Widget*> && ...)
  FlexBox* add(Elements*...);
};

template <class... Elements>
    requires (std::convertible_to<Elements*, Widget*> && ...)
FlexBox* FlexBox::add(Elements*... btns)
{
  static_assert(sizeof...(Elements) > 0);
  let anchorer = [this](Widget& w, Widget* prev = nullptr)
  {
    if (orientation() == HORIZONTAL)
    {
      w.top_in(this, -padding, true);
      w.bottom_in(this, -padding, true);
    }
    else
    {
      w.left_in(this, -padding, true);
      w.right_in(this, -padding, true);
    }
    switch (dir)
    {
    case L2R:
      if (prev)
        w.right_of(prev, CENTER_Y, margin);
      else
        w.left_in(this, -padding);
      break;
    case R2L:
      if (prev)
        w.left_of(prev, CENTER_Y, margin);
      else
        w.right_in(this, padding);
      break;
    case T2B:
      if (prev)
        w.put_under(prev, CENTER_X, margin);
      else
        w.top_in(this, padding);
      break;
    case B2T:
      if (prev)
        w.put_above(prev, CENTER_X, margin);
      else
        w.bottom_in(this, padding);
      break;
    }
  };
  let start = children.size();
  Layout::add(btns...);
  if (start == 0)
    anchorer(*children[0]);
  let prev_index = start == 0 ? 0 : start-1;
  Widget* prev = children[prev_index].get();
  for (size_t i = prev_index+1; i < children.size(); i++)
  {
    anchorer(*children[i], prev);
    prev = children[i].get();
  }
  Anchor* anch;
  if (anchor_p == 0)
    anch = &push_anchor();
  else
    anch = anchors+0;
  switch (dir)
  {
    case L2R:
      anch->anchor(this, RIGHT).on(children.back().get(), RIGHT)
        .margin(padding).resizable();
      break;
    case R2L:
      anch->anchor(this, LEFT).on(children.back().get(), LEFT)
        .margin(padding).resizable();
      break;
    case T2B:
      anch->anchor(this, BOTTOM).on(children.back().get(), BOTTOM)
        .margin(padding).resizable();
      break;
    case B2T:
      anch->anchor(this, TOP).on(children.back().get(), TOP)
        .margin(padding).resizable();
      break;
  }
  return this;
}

