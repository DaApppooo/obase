#pragma once
#include "fader.hpp"
#include "raylib.h"
#include "widget.hpp"
#include <functional>
#include <string_view>
#include "hull.hpp"

enum ButtonState
{
  BTN_RELEASED,
  BTN_HOVERED,
  BTN_DOWN,
  BTN_LOCKED // Used with ToggleButton
};

struct ButtonStyle
{
  f32 roundness_left, roundness_right;

  ButtonStyle()
    : roundness_left(0.1f), roundness_right(0.1f)
  {}
  ButtonStyle(const ButtonStyle&) = default;
  ButtonStyle(f32 left, f32 right)
    : roundness_left(left),
      roundness_right(right)
  {}
};

Color button_bg_style(ButtonState);
Color button_border_style(ButtonState);

struct PushButton : Hull
{
  std::function<void(MouseButton)> _on_click;
  ButtonStyle style;
  ButtonState old_state;
  ButtonState state;
  BaseFader fader;
  
  PushButton(std::string&& id, std::function<void(MouseButton)>&& on_click = nullptr);

  PushButton* set_on_click(std::function<void(MouseButton)>&& on_click);

  inline void set_state(ButtonState new_state);

  inline PushButton* set_roundness(f32 roundness)
  { style.roundness_left = style.roundness_right = roundness; return this; }
  inline PushButton* set_left_roundness(f32 r)
  { style.roundness_left = r; return this; }
  inline PushButton* set_right_roundness(f32 r)
  { style.roundness_right = r; return this; }
  PushButton* set_text(std::string&& txt);

  Widget* request(std::string_view name) override;
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_leave() override;
  void on_release(MouseButton) override;

  void debug_draw() override;
  void draw() override;
  ~PushButton() override;
};

inline void PushButton::set_state(ButtonState new_state)
{
  if (new_state == state)
    return;
  fader.set();
  old_state = state;
  state = new_state;
}

