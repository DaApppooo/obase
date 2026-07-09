#pragma once
#include "anchor.hpp"
#include "app.hpp"
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

FORMATTER(ButtonState, [](auto& out, ButtonState state)
{
  switch (state)
  {
    ESTR(BTN_RELEASED)
    ESTR(BTN_HOVERED)
    ESTR(BTN_DOWN)
    ESTR(BTN_LOCKED)
  }
});

Color button_bg_style(ButtonState);
Color button_border_style(ButtonState);

struct BaseButtonStyle
{
  ButtonState state, old_state;
  BaseFader fader;

  BaseButtonStyle()
    : state(BTN_RELEASED),
      old_state(BTN_RELEASED),
      fader()
  {}
  BaseButtonStyle(const BaseButtonStyle&) = default;

  inline bool update(f32 dt) { return fader.update(dt); }
  inline Color bg() const
  { return fader.get(button_bg_style(old_state), button_bg_style(state)); }
  inline Color border() const
  { return fader.get(button_border_style(old_state), button_border_style(state)); }
  inline void set_state(ButtonState new_state)
  {
    if (new_state == state)
      return;
    fader.set();
    old_state = state;
    state = new_state;
  }

  inline void on_click(Widget* self, MouseButton);
  inline void on_hover();
  inline void on_leave();
  inline void on_release(
    Widget* self,
    std::function<void()> on_hover_and_release,
    MouseButton,
    bool can_unfocus = true
  );
};

struct ButtonStyle
{
  f32 roundness_left, roundness_right;
  BaseButtonStyle base;

  ButtonStyle()
    : roundness_left(0.1f), roundness_right(0.1f)
  {}
  ButtonStyle(const ButtonStyle&) = default;
  ButtonStyle(f32 left, f32 right)
    : roundness_left(left),
      roundness_right(right)
  {}

  inline ButtonState state() const { return base.state; }
  inline Color bg() const { return base.bg(); }
  inline Color border() const { return base.border(); }
  inline void set_state(ButtonState s) { base.set_state(s); }
  inline bool update(f32 dt) { return base.update(dt); }
};

struct PushButton : Hull
{
  std::function<void()> _on_click;
  ButtonStyle style;
  
  PushButton(std::string&& id, std::function<void()>&& on_click = nullptr);

  PushButton* set_on_click(std::function<void()>&& on_click);

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

  void draw() override;
  ~PushButton() override;
};

inline void BaseButtonStyle::on_click(Widget* self, MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (state == BTN_LOCKED)
    return;
  // if (app().focused)
  //   return;
  set_state(BTN_DOWN);
  // button will continue to recieve events even if the cursor isn't over the object
  focus(self);
}

inline void BaseButtonStyle::on_hover()
{
  if (state == BTN_RELEASED)
  {
    set_state(BTN_HOVERED);
  }
}

inline void BaseButtonStyle::on_leave()
{
  if (state == BTN_HOVERED)
  {
    set_state(BTN_RELEASED);
  }
}

inline void BaseButtonStyle::on_release(
  Widget* self,
  std::function<void()> on_hover_and_release,
  MouseButton btn,
  bool can_unfocus
) {
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (state != BTN_DOWN && state != BTN_LOCKED)
    return;
  if (CheckCollisionPointRec(GetMousePosition(), self->rect))
  {
    if (on_hover_and_release)
      on_hover_and_release();
    if (state != BTN_LOCKED)
      set_state(BTN_HOVERED);
  }
  else
    set_state(BTN_RELEASED);
  if (can_unfocus)
    unfocus(self);
}

