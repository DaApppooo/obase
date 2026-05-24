#pragma once
#include "menu.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "text.hpp"
#include "widget.hpp"

template <class T>
struct ContextMenu : T
{
  Menu menu;
  bool just_clicked;

  ContextMenu(T&& inner);

  void on_hover() override;
  void on_click(MouseButton) override;
  void on_double_click() override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_scroll() override;
  void on_unfocus() override;
  
  void debug_draw() override;
  void draw() override;
  void update() override;

  Widget* request(std::string_view name) override;

  template <class Func>
  ContextMenu<T>* map(Func&& f)
  { f(static_cast<T*>(this)); return this; }

  ~ContextMenu() override = default;
};

inline mut make_menu_button = [](std::string&& name, std::string&& text) static -> Own<PushButton*>
{
  static std::string buffer;
  buffer.clear();
  buffer += name;
  mut btn = (new PushButton(std::move(name)));
  btn
    ->set_on_click(
      // note: btn is a pointer, we can take it by copy !
      [btn](MouseButton) mutable
      {
        mut* menu = btn->child->as<Layout>()->children[1]->as<Menu>();
        menu->top(btn->bottom());
        menu->left(btn->left());
        menu->show();
        focus(menu);
      }
    )
    ->add(
      (new Layout(std::format("{}.inner", buffer)))
      ->add(
        (new Text(std::move(text))),
        (new Menu(std::format("{}.menu", buffer)))
      )
    );
  btn->child->fill_parent()->as<Layout>()->children[0]->fill_parent();
  btn->child->as<Layout>()->children[1]->hide();
  return btn;
};

template <class T>
ContextMenu<T>::ContextMenu(T&& inner)
  : T(std::move(inner)),
    menu("")
{
  menu._name = std::format("{}.context_menu", this->_name);
  menu.parent = this;
  println("The menu name is '{}'", menu._name);
  menu.hide();
}

#define TRANSFER(PRE_HANDLE, BTN_TYPE, BTN, FUNC) \
template <class T> \
void ContextMenu<T>::FUNC(BTN_TYPE BTN) \
{ \
  PRE_HANDLE; \
  T::FUNC(BTN); \
  menu.FUNC(BTN); \
}

TRANSFER(
  if (btn == MOUSE_RIGHT_BUTTON)
  {
    menu.show();
    let mpo = GetMousePosition();
    menu.x(mpo.x);
    menu.y(mpo.y);
    if (menu.w() == 0)
      menu.w(1);
    focus(&menu);
    app().redraw();
    just_clicked = true;
    return;
  }
  , MouseButton, btn, on_click)
TRANSFER(,MouseButton,btn, on_release)
TRANSFER(,MouseButton,btn, on_drag)
TRANSFER(,,, on_unfocus)
TRANSFER(,,, on_hover)
TRANSFER(,,, on_double_click)
TRANSFER(,,, on_scroll)
TRANSFER(,,, debug_draw)
TRANSFER(,,, draw)
TRANSFER(
,,, update)

template <class T>
Widget* ContextMenu<T>::request(std::string_view name)
{
  mut* p = T::request(name);
  if (p)
    return p;
  if (name == menu._name)
    return &menu;
  return menu.request(name);
}

#undef TRANSFER

