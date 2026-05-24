#pragma once
#include "sys_settings.hpp"
#include "toggle_button.hpp"

struct ToggleIcon : ToggleButton
{
  Icon off_icon;
  Icon on_icon;
  
  ToggleIcon(std::string&& name, std::function<void(MouseButton, bool)>&& on_click);

  void draw() override;
  
};

