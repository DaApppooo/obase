#include "text_box.hpp"
#include "app.hpp"
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
{ style.draw(this, CENTER_Y); }

void MonoTextBox::update()
{
  style.update(this);
  Widget::update();
}

// MultiTextBox::MultiTextBox(std::string&& name)
//   : Widget(std::move(name))
// {}
