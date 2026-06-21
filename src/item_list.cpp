#include "item_list.hpp"
#include "app.hpp"
#include "raylib.h"
#include "scroll_bar.hpp"
#include "sys_settings.hpp"
#include "widget.hpp"

ItemList::ItemList(std::string&& id)
  : Widget(std::move(id)),
    selected_index(0),
    entries(),
    scrollbar(),
    expand_height(VARIABLE_SIZE),
    padding(5.f)
{
  scrollbar.orient = VERTICAL;
}

void ItemList::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  let mpo = GetMousePosition();
  let entry_size = palette().text_size + 2.f*padding;
  if (opened())
  {
    assert(is_focused(this));
    if (
       CheckCollisionPointRec(mpo, {x(), y(), w(), entry_size})
    or not CheckCollisionPointRec(mpo, rect)
    ) {
      app().redraw();
      UNFOCUS_ME;
      return;
    }
    i64 hovering;
    let scroll_rect = interior_scrollbar();
    if (should_show_scrollbar())
    {
      let mpo_relative = mpo.y - rect.y - entry_size;
      let offset_mpo = mpo_relative
                        + (1.f-scrollbar.value(scroll_rect))
                          * interior_total_height();
      hovering = offset_mpo / entry_size;
    }
    else
    {
      let mpo_relative = mpo.y - rect.y - entry_size;
      hovering = mpo_relative / entry_size;
    }
    if (hovering >= 0 and hovering < entries.size())
    {
      if (entries[hovering].on_select)
        entries[hovering].on_select(entries[hovering].text);
      selected_index = hovering;
      UNFOCUS_ME;
    }
    else if (
        should_show_scrollbar()
    and CheckCollisionPointRec(mpo, scroll_rect)
    ) {
      scrollbar.on_click(this, btn);
    }
  }
  else
  {
    assert(!is_focused(this));
    open();
  }
  app().redraw();
}

void ItemList::on_hover()
{
  if (!opened())
    return;
  app().redraw();
  if (!should_show_scrollbar())
    return;
  let scroll = interior_scrollbar();
  if (CheckCollisionPointRec(GetMousePosition(), scroll))
    scrollbar.on_hover(this);
}
void ItemList::on_leave()
{
  if (!opened())
    return;
  if (!should_show_scrollbar())
    return;
  let scroll = interior_scrollbar();
  if (CheckCollisionPointRec(GetMousePosition(), scroll))
    scrollbar.on_leave(this);
}
void ItemList::on_drag(MouseButton)
{
  if (!opened())
    return;
  if (!should_show_scrollbar())
    return;
  let scroll = interior_scrollbar();
  if (CheckCollisionPointRec(GetMousePosition(), scroll))
    scrollbar.on_drag(this);
}
void ItemList::on_release(MouseButton btn)
{
  if (!opened())
    return;
  if (!should_show_scrollbar())
    return;
  let scroll = interior_scrollbar();
  if (CheckCollisionPointRec(GetMousePosition(), scroll))
    scrollbar.on_release(this, btn);
}

void ItemList::on_unfocus()
{
  close();
}

void ItemList::on_scroll()
{
  if (!opened())
    return;
  if (
      should_show_scrollbar()
  and CheckCollisionPointRec(GetMousePosition(), rect)
  ) {
    scrollbar.on_scroll(this);
  }
}

void ItemList::update()
{
  flags |= W_LOCKED | H_LOCKED;
  if (is_focused(this))
    assert(opened());
  if (rect.height == 0)
    rect.height = palette().text_size + padding*2.f;
  if (opened() and should_show_scrollbar())
    scrollbar.update(rect);
  Widget::update();
}

void ItemList::draw()
{
  let roundness = 0.2f;
  let entry_h = (palette().text_size + 2.f*padding);
  { // opening box
    let r = Rect{x(), y(), w(), entry_h};
    let old = app().scissor_begin(r);
    DrawRoundedBox(
      r, roundness, roundness, 1,
      palette().border(UI_DEFAULT),
      palette().bg(UI_DEFAULT)
    );

    palette().draw_icon(
      ICON_ARROW_HEAD,
      reduce({
        r.x+r.width-r.height,
        r.y,
        r.height,
        r.height
      }, 5),
      opened() ? -90 : 90,
      palette().border(UI_DEFAULT)
    );
    
    if (selected_index < entries.size())
    {
      let str = entries[selected_index].text.c_str();
      let m = MeasureTextEx(
        font(),
        str,
        palette().text_size,
        2.f
      );
      DrawTextEx(
        font(),
        str,
        {
          r.x+padding, // align left
          r.y+padding+(r.height-padding*2)/2-m.y/2 // center y-wise
        },
        palette().text_size,
        2.f,
        palette().text
      );
    }
    app().scissor_end(old);
  }
  
  if (opened())
  {
    for (size_t i = 0; i < entries.size(); i++)
    {
      let r = Rect{
        x(),
        y() + (i+1)*entry_h,
        w(),
        entry_h
      };
      let hovered = CheckCollisionPointRec(GetMousePosition(), r);
      let old = app().scissor_begin(r);
      DrawRoundedBox(
        r,
        0, 0, 1,
        palette().border(UI_INACTIVE),
        palette().bg(hovered ? UI_ACTIVE : UI_DEFAULT)
      );
      let str = entries[i].text.c_str();
      let m = MeasureTextEx(font(), str, palette().text_size, 2.f);
      DrawTextEx(
        font(),
        str,
        {
          r.x+padding, // align left
          r.y+r.height/2.f-m.y/2.f // center y-wise
        },
        palette().text_size,
        2.f,
        palette().text
      );
      app().scissor_end(old);
    }
    if (should_show_scrollbar())
    {
      scrollbar.draw({
        x(),
        y() + entry_h,
        ScrollBarStyle::BUTTON_MIN_SIZE,
        h() - entry_h
      });
    }
  }
}

ItemList::~ItemList()
{ }
