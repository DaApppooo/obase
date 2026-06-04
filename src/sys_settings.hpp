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
  ICON_CLOSE,
  ICON_ARROW_HEAD, // points right
  ICON_FULLSCREEN,
  ICON_MINIMIZE,
  ICON_COUNT
};
using IconBuffer = Texture[ICON_COUNT];

float double_click_time();
Theme system_theme();
void load_icons(IconBuffer& buffer);

// Drawing utilities extension:

void DrawRectangleRoundedGradientH(
  Rectangle rec,
  float roundnessLeft,
  float roundnessRight,
  int segments,
  Color left,
  Color right
);
inline void DrawRectangleRoundedPro(  
  Rectangle rec,
  float roundnessLeft,
  float roundnessRight,
  int segments,
  Color color
) {
  DrawRectangleRoundedGradientH(rec, roundnessLeft, roundnessRight, segments,
                                color, color);
}

Image load_svg_to_img(const char *path, int w, int h);
inline Texture load_svg(const char* path, int w, int h)
{
  Image img = load_svg_to_img(path, w, h);
  Texture tex = LoadTextureFromImage(img);
  UnloadImage(img);
  return tex;
}
