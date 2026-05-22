#pragma once
#include "menu.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "text.hpp"

template <class T>
struct ContextMenu : Menu
{
  T inner;

  ContextMenu(T&& inner);

  void on_hover() override;
  void on_click(MouseButton) override;
  void on_double_click() override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_scroll() override;
  
  void debug_draw() override;
  void draw() override;
  void update() override;

  template <class Func>
  void map(Func&& f)
  { std::forward<Func>(&f)(inner); }

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
      [btn, held_name = std::string(buffer)](MouseButton) mutable
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
  : Menu(std::format("{}.context_menu", inner._name)),
    inner(std::move(inner))
{}

#define TRANSFER(PRE_HANDLE, BTN_TYPE, BTN, FUNC) \
template <class T> \
void ContextMenu<T>::FUNC(BTN_TYPE BTN) \
{ \
  PRE_HANDLE; \
  if (visible()) \
    Menu::FUNC(BTN); \
  inner.FUNC(BTN); \
}

TRANSFER(if (btn == MOUSE_RIGHT_BUTTON) show(), MouseButton, btn, on_click)
TRANSFER(,MouseButton,btn, on_release)
TRANSFER(,MouseButton,btn, on_drag)
TRANSFER(,,, on_hover)
TRANSFER(,,, on_scroll)
TRANSFER(,,, debug_draw)
TRANSFER(,,, draw)
TRANSFER(,,, update)

#undef TRANSFER

