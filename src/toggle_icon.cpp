#include "toggle_icon.hpp"
#include "app.hpp"
#include "raylib.h"
#include "sys_settings.hpp"
#include "toggle_button.hpp"


ToggleIcon::ToggleIcon(std::string&& name, std::function<void(bool)>&& on_click)
  : ToggleButton(std::move(name), std::move(on_click)),
    off_icon(ICON_CLOSE), on_icon(ICON_CLOSE)
{}
    
void ToggleIcon::draw()
{
  if (!visible())
    return;
  assert(!child);
  Texture tex = palette().icons[is_on() ? on_icon : off_icon];
  DrawTexturePro(
    tex,
    TEX_ORIGINAL_RECT(tex),
    rect,
    {0.f, 0.f},
    0.f,
    style.border()
  );
  if (style.update(GetFrameTime()))
    app().redraw();
}

