#pragma once
#include "layout.hpp"
#include "push_button.hpp"
#include "toggle_button.hpp"

struct ButtonGroup : Layout
{ 
  ButtonGroup(std::string&& id);

  template <class... Btns>
    requires (TypeSet<PushButton, ToggleButton>::is_present<Btns> && ...)
  ButtonGroup* add(Btns*...);
};

template <class... Btns>
  requires (TypeSet<PushButton, ToggleButton>::is_present<Btns> && ...)
ButtonGroup* ButtonGroup::add(Btns*... btns)
{
  Layout::add(btns...);
  children[0]->left_in(this);
  children[0]->top_in(this);
  children[0]->bottom_in(this);
  Widget* prev = children[0].get();
  for (size_t i = 0; i < children.size(); i++)
  {
    children[i]->left_of(prev, CENTER_Y);
    children[i]->top_in(this);
    children[i]->bottom_in(this);
    prev = children[i].get();
  }
  return this;
}

