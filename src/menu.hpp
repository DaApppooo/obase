#pragma once
#include "app.hpp"
#include "flexbox.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "sys_settings.hpp"
#include "raymath.h"

struct Menu;

struct MenuEntry : Widget
{
  static macro f32 padding = 5.f;

  std::optional<Icon> icon;
  std::string text;
  uptr<Menu> submenu;
  std::function<void()> _on_click;
  BaseButtonStyle style;
  bool _enabled;

  MenuEntry(std::string&& id, std::string&& text);

  inline MenuEntry* set_on_click(std::function<void()>&& on_click)
  { _on_click = std::move(on_click); return this; }
  inline MenuEntry* set_icon(Icon ic)
  { icon = ic; return this; }
  inline MenuEntry* set_submenu(Own<Menu*> submenu_);
  inline void disable() { _enabled = false; }
  inline void enable() { _enabled = true; }
  inline bool enabled() const { return _enabled; }
  inline Vec2 min_size() const
  {
    mut min_s = MeasureTextEx(font(), text.c_str(), palette().text_size, 2.f);
    min_s = min_s + Vec2{padding, padding}*2.f;
    if (icon)
    {
      let tex = palette().icons[*icon];
      let ratio = f32(tex.height)/tex.width;
      let draw_h = (min_s.y-padding*2.f);
      min_s.x = draw_h*ratio + padding;
    }
    if (submenu)
    {
      min_s.x += palette().largest_digit_w;
    }
    return min_s;
  }

  Widget* request(std::string_view name) override;
  void on_click(MouseButton) override;
  void on_hover() override;
  void on_leave() override;
  void on_release(MouseButton) override;
  
  void update() override;
  void draw() override;
  void debug_draw() override;
  
  ~MenuEntry() override;
};

struct Menu : FlexBox
{
  Menu(std::string&& id);

  inline Menu* entry(std::string&& text, std::function<void()>&& func)
  {
    add(
      (new MenuEntry(indexed_id(std::string(_name), children.size()), std::move(text)))
      ->set_on_click(std::move(func))
    );
    return this;
  }
  inline Menu* entry(
    Icon icon,
    std::string&& text,
    std::function<void()>&& func
  ) {
    add(
      (new MenuEntry(indexed_id(std::string(_name), children.size()), std::move(text)))
      ->set_icon(icon)
      ->set_on_click(std::move(func))
    );
    return this;
  }
  inline Menu* entry(
    std::string&& text,
    Own<Menu*> submenu
  ) {
    add(
      (new MenuEntry(indexed_id(std::string(_name), children.size()), std::move(text)))
      ->set_submenu(submenu)
    );
    submenu->hide();
    return this;
  }
  inline Menu* entry(
    Icon icon,
    std::string&& text,
    Own<Menu*> submenu
  ) {
    add(
      (new MenuEntry(indexed_id(std::string(_name), children.size()), std::move(text)))
      ->set_icon(icon)
      ->set_submenu(submenu)
    );
    submenu->hide();
    return this;
  }
  inline bool is_inside()
  {
    bool inside = CheckCollisionPointRec(GetMousePosition(), rect);
    if (inside)
      return true;
    for (mut& w : children)
    {
      mut* entry = static_cast<MenuEntry*>(w.get());
      if (entry->submenu && entry->submenu->is_inside())
        return true;
    }
    return false;
  }

  void on_click(MouseButton) override;
  void on_unfocus() override;
  void draw() override;

  ~Menu() override;
};

inline MenuEntry* MenuEntry::set_submenu(Own<Menu*> submenu_)
{
  submenu.reset(submenu_);
  submenu->parent = this;
  submenu->hide();
  return this;
}

