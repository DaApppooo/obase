/**
  @brief System dependent settings.
*/

#pragma once

#include "raylib.h"
enum Theme
{
  THEME_DARK,
  THEME_LIGHT
};

enum Icon
{
  CROSS,
  ICON_COUNT
};
using IconBuffer = Texture[ICON_COUNT];

float double_click_time();
Theme system_theme();
void load_icons(IconBuffer& buffer);

