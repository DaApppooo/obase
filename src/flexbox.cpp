#include "flexbox.hpp"

FlexBox::FlexBox(std::string&& id)
  : Layout(std::move(id)),
    dir(L2R)
{}

void FlexBox::update()
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
    child->update();
  old_rect = rect;
}

FlexBox* FlexBox::set_padding(f32 pad)
{
  padding = pad;
  for (mut& w : children)
  {
    w->anchors[0].margin(-pad); // TOP
    w->anchors[1].margin(-pad); // BOTTOM
  }
  if (children.size())
    children[0]->anchors[2].margin(pad);
  if (anchor_p)
    anchors[0].margin(pad);
  return this;
}

FlexBox* FlexBox::set_margin(f32 marg)
{
  margin = marg;
  for (size_t i = 1; i < children.size(); i++)
  {
    children[i]->anchors[2].margin(marg);
  }
  return this;
}

void FlexBox::on_leave()
{
  for (mut& child : children)
  {
    child->on_leave();
  }
}




