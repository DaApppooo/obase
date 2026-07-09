#include "menu.hpp"
#include "app.hpp"
#include "flexbox.hpp"
#include "raylib.h"

MenuEntry::MenuEntry(std::string&& id, std::string&& text)
  : Widget(std::move(id)),
    icon(), text(std::move(text)),
    submenu(nullptr), _on_click(nullptr),
    style(), _enabled(true)
{}

Widget* MenuEntry::request(std::string_view name)
{
  if (!submenu)
    return nullptr;
  if (submenu->_name == name)
    return submenu.get();
  return submenu->request(name);
}

void MenuEntry::on_click(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  style.set_state(BTN_DOWN);
  // button will continue to recieve events even if the cursor isn't over the object
  FOCUS_ME;
}

void MenuEntry::on_hover()
{
  if (style.state == BTN_RELEASED)
  {
    style.set_state(BTN_HOVERED);
  }
}

void MenuEntry::on_leave()
{
  if (style.state == BTN_HOVERED)
  {
    style.set_state(BTN_RELEASED);
  }
}

void MenuEntry::on_release(MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (style.state != BTN_DOWN)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), rect))
  {
    if (submenu)
    {
      submenu->x(right());
      submenu->y(top());
      submenu->show();
      focus(submenu.get());
    }
    if (_on_click)
      _on_click();
    style.set_state(BTN_HOVERED);
  }
  else
    style.set_state(BTN_RELEASED);
  if (app().focused == this)
    focus(parent); // bring focus back to menu
}

void MenuEntry::update()
{
  Widget::update();
  let s = min_size();
  if (w() < s.x)
    w(s.x);
  if (h() < s.y)
    h(s.y);
  if (submenu && submenu->visible())
  {
    submenu->update();
  }
}

void MenuEntry::debug_draw()
{
  Widget::debug_draw();
  if (submenu)
    submenu->debug_draw();
}

void MenuEntry::draw()
{
  macro i32 segments = 8;
  macro f32 thickness = 1.f;
  macro f32 roundness = 0.1f;

  let old = app().scissor_begin(
    {0.f, 0.f, f32(GetScreenWidth()), f32(GetScreenHeight())},
    false
  );
  
  DrawRectangleRoundedPro(
    rect,
    roundness,
    roundness,
    segments,
    style.bg()
  );
  mut pos = rect.x + padding;
  if (icon)
  {
    let tex = palette().icons[*icon];
    let ratio = f32(tex.height)/tex.width;
    let draw_h = (rect.height-padding*2.f);
    DrawTexturePro(
      tex,
      { 0.f, 0.f, f32(tex.width), f32(tex.height) },
      { pos, rect.y+padding, ratio*draw_h, draw_h },
      {0.f, 0.f}, 0.f, palette().text
    );
    pos += ratio*draw_h + padding;
  }
  let m = MeasureTextEx(font(), text.c_str(), palette().text_size, 2.f).y;
  let draw_y = y() + h()/2.f - m/2.f;
  DrawTextEx(
    font(), text.c_str(), {pos, draw_y}, palette().text_size,
    2.f, palette().text
  );
  style.update(GetFrameTime());
  if (submenu)
  {
    DrawTextEx(
      font(), ">", { x()+w()-palette().largest_digit_w, draw_y },
      palette().text_size, 2.f, palette().border(UI_INACTIVE)
    );
    submenu->draw();
  }
  app().scissor_end(old);
}

MenuEntry::~MenuEntry() {}

// === Menu === //

Menu::Menu(std::string&& id)
  : FlexBox(std::move(id))
{
  set_direction(T2B);
  set_padding(0);
  set_margin(0);
}

void Menu::draw()
{
  if (!_visible)
    return;
  f32 max_w = 0.f;
  for (mut& child : children)
  {
    if (child->w() > max_w)
      max_w = child->w();
  }
  for (mut& child : children)
  {
    child->w(max_w);
  }
  w(max_w + margin*2.f);
  FlexBox::draw();
}

void Menu::on_click(MouseButton btn)
{
  if (!CheckCollisionPointRec(GetMousePosition(), rect))
  {
    hide();
    UNFOCUS_ME;
    parent->if_is<MenuEntry>([btn](MenuEntry* entry){
      Menu* parent_menu = entry->parent->as<Menu>();
      parent_menu->on_click(btn);
    });
  }
  FlexBox::on_click(btn);
}

void Menu::on_unfocus()
{
  hide();
}

Menu::~Menu() {}
