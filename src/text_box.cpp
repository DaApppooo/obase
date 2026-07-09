#include "text_box.hpp"
#include "app.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "scroll_bar.hpp"
#include "widget.hpp"

MonoTextBox::MonoTextBox(std::string&& name)
  : Widget(std::move(name)),
    _on_validate(nullptr),
    style()
{}

void MonoTextBox::on_hover()
{ style.on_hover(); }

void MonoTextBox::on_leave()
{ style.on_leave(); }

void MonoTextBox::on_click(MouseButton btn)
{ style.on_click(this, btn); }

void MonoTextBox::on_drag(MouseButton btn)
{ style.on_drag(this, btn); }

void MonoTextBox::on_release(MouseButton btn)
{ style.on_release(this, btn); }

void MonoTextBox::on_keydown(u16 key)
{
  if (key == '\r')
  {
    validate();
    UNFOCUS_ME;
    return;
  }
  style.on_keydown(this, key);
}

void MonoTextBox::on_keyup(u16 key)
{ style.on_keyup(this, key); }

void MonoTextBox::draw()
{ style.draw(this, CENTER_Y, 0.f); }

void MonoTextBox::update()
{
  style.update(this);
  Widget::update();
}

MultiTextBox::MultiTextBox(std::string&& name)
  : Widget(std::move(name)),
    style(),
    scrollbar()
{
  scrollbar.orient = VERTICAL;
}

void MultiTextBox::on_hover()
{
  if (
    shows_scrollbar()
    and CheckCollisionPointRec(
      GetMousePosition(),
      scrollbar_rect()
    )
  )
    scrollbar.on_hover(this);
  else
    style.on_hover();
}

void MultiTextBox::on_leave()
{
  let mpo = GetMousePosition();
  let prev = mpo - GetMouseDelta();
  if (
    shows_scrollbar()
    and not CheckCollisionPointRec(
      mpo,
      scrollbar_rect()
    )
    and CheckCollisionPointRec(
      prev,
      scrollbar_rect()
    )
  )
    scrollbar.on_leave(this);
  style.on_leave();
}

void MultiTextBox::on_click(MouseButton btn)
{
  if (
    shows_scrollbar()
    and CheckCollisionPointRec(
      GetMousePosition(),
      scrollbar_rect()
    )
  )
    scrollbar.on_click(this, btn);
  else
    style.on_click(this, btn);
}

void MultiTextBox::on_drag(MouseButton btn)
{
  if (
    shows_scrollbar()
    and (
      CheckCollisionPointRec(
        GetMousePosition(),
        scrollbar_rect()
      )
      or scrollbar.handle_style.state == BTN_DOWN
    )
  )
    scrollbar.on_drag(this);
  else
    style.on_drag(this, btn);
}

void MultiTextBox::on_release(MouseButton btn)
{
  if (shows_scrollbar())
    scrollbar.on_release(this, btn, false);
  style.on_release(this, btn);
}

void MultiTextBox::on_keydown(u16 key)
{
  style.on_keydown(this, key);
}

void MultiTextBox::on_keyup(u16 key)
{ style.on_keyup(this, key); }

void MultiTextBox::on_scroll()
{
  let r = scrollbar_rect();
  let delta = GetMouseWheelMove();
  scrollbar.value(
    r,
    scrollbar.value(r) - delta*scrollbar.zoom(r)*0.1f
  );
  scrollbar.clamp(r);
}

void MultiTextBox::draw()
{
  if (shows_scrollbar())
  {
    let r = Rect{
      rect.x + rect.width - ScrollBarStyle::BUTTON_MIN_SIZE,
      rect.y,
      ScrollBarStyle::BUTTON_MIN_SIZE,
      rect.height
    };
    style.draw(this, TOP, scrollbar.value(r));
    scrollbar.draw(r);
  }
  else
    style.draw(this, TOP, 0.f);
}

void MultiTextBox::update()
{
  style.update(this);
  Widget::update();
  if (shows_scrollbar())
  {
    let r = Rect{
      rect.x + rect.width - ScrollBarStyle::BUTTON_MIN_SIZE,
      rect.y,
      ScrollBarStyle::BUTTON_MIN_SIZE,
      rect.height
    };
    scrollbar.draw(r);
    scrollbar.handle.height = std::max(scrollbar.handle.height, 20.f);
    scrollbar.zoom(r, h()/style.measure_text().y);
  }
}


