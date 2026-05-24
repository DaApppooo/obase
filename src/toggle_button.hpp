#pragma once
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
  
  ToggleButton(
    std::string&& id,
    std::function<void(MouseButton, bool down)>&& on_click = nullptr
  );

  ToggleButton(ToggleButton&&) = default;

  inline void turn(bool state);
  inline void turn_on() { turn(true); }
  inline void turn_off() { turn(false); }
  inline bool is_on() const { return style.base.state == BTN_LOCKED; }

  ToggleButton* set_on_click(std::function<void(MouseButton, bool)>&& on_click);
  ToggleButton* add(Own<Widget*> child);

  ToggleButton* set_text(std::string&& txt);
  // void set_icon()

  Widget* request(std::string_view name) override;
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_leave() override;
  void on_release(MouseButton) override;

  void draw() override;
  ~ToggleButton() override;
};

inline void ToggleButton::turn(bool state)
{ style.base.state = state ? BTN_LOCKED : BTN_RELEASED; }
