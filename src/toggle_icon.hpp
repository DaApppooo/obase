#pragma once
#include "sys_settings.hpp"
#include "toggle_button.hpp"

struct ToggleIcon : ToggleButton
{
  Icon off_icon;
  Color off_color;
  Icon on_icon;
  Color on_color;
  Fader<f32> hovering;
  
  ToggleIcon(std::string&& name, std::function<void(bool)>&& on_click = nullptr);

  inline ToggleIcon* when_on(Icon icon, Color color)
  { on_icon = icon; on_color = color; return this; }
  inline ToggleIcon* when_off(Icon icon, Color color)
  { off_icon = icon; off_color = color; return this; }

  void on_hover() override;
  void on_leave() override;
  void draw() override;
  
};

