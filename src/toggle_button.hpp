#pragma once
#include "fader.hpp"
#include "raylib.h"
#include "widget.hpp"
#include <functional>
#include <string_view>
#include "push_button.hpp"

Color button_bg_style(ButtonState);
Color button_border_style(ButtonState);

struct ToggleButton : Hull
{
  std::function<void(MouseButton, bool)> _on_click;
  ButtonStyle style;
  ButtonState old_state;
  ButtonState state;
  BaseFader fader;
  
  ToggleButton(
    std::string&& id,
    std::function<void(MouseButton, bool down)>&& on_click = nullptr
  );

  ToggleButton* set_on_click(std::function<void(MouseButton, bool)>&& on_click);
  ToggleButton* add(Own<Widget*> child);

  inline void set_state(ButtonState new_state);

  ToggleButton* set_text(std::string&& txt);
  // void set_icon()

  Widget* request(std::string_view name) override;
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_leave() override;
  void on_release(MouseButton) override;

  void debug_draw() override;
  void update() override;
  void draw() override;
  ~ToggleButton() override;
};

inline void ToggleButton::set_state(ButtonState new_state)
{
  if (new_state == state)
    return;
  fader.set();
  old_state = state;
  state = new_state;
}

