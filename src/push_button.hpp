#pragma once
#include "fader.hpp"
#include "raylib.h"
#include "widget.hpp"
#include <functional>
#include <string_view>

enum ButtonState
{
  BTN_RELEASED,
  BTN_HOVERED,
  BTN_DOWN
};

Color button_bg_style(ButtonState);
Color button_border_style(ButtonState);

struct PushButton : Widget
{
  std::function<void(MouseButton)> _on_click;
  uptr<Widget> child;
  Rect old_rect;
  ButtonState old_state;
  ButtonState state;
  BaseFader fader;
  
  PushButton(std::string&& id, std::function<void(MouseButton)>&& on_click = nullptr);

  PushButton* set_on_click(std::function<void(MouseButton)>&& on_click);
  PushButton* add(Own<Widget*> child);

  inline void set_state(ButtonState new_state);

  PushButton* set_text(std::string&& txt);
  // void set_icon()

  Widget* request(std::string_view name) override;
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_release(MouseButton) override;

  void update() override;
  void draw() override;
  ~PushButton() override;
};

inline void PushButton::set_state(ButtonState new_state)
{
  if (new_state == old_state)
    return;
  fader.set();
  old_state = state;
  state = new_state;
}

