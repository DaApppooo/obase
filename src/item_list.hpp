#pragma once
#include "app.hpp"
#include "raylib.h"
#include "scroll_bar.hpp"
#include "widget.hpp"
#include <string_view>

// Note: no need to make it more modular given we'll only need it for small lists of items.
struct ItemList : Widget
{
  struct Item
  {
    std::string text;
    std::function<void(std::string_view option)> on_select;
  };
  size_t selected_index;
  std::vector<Item> entries;
  ScrollBarStyle scrollbar;
  f32 expand_height;
  f32 padding;

  ItemList(std::string&& id);

  inline ItemList* add(std::string&& text, decltype(Item::on_select) on_select = nullptr)
  {
    if (selected_index >= entries.size())
      ++selected_index;
    entries.push_back({std::move(text), std::move(on_select)});
    return this;
  }

  inline ItemList* expands_to(f32 height_when_expanded)
  { expand_height = height_when_expanded; return this; }

  inline ItemList* default_option(size_t index)
  { selected_index = index; return this; }
  inline ItemList* default_option(std::string_view text)
  {
    u64 i = 0;
    for (let& entry : entries)
    {
      if (entry.text == text)
      {
        selected_index = i;
        return this;
      }
      ++i;
    }
    assert("Specified option does not exists.");
    return this;
  }
  
  inline f32 interior_total_height() const
  { return entries.size() * (palette().text_size + 2*padding); }
  inline Rect interior_rect() const
  {
    return {
      rect.x,
      rect.y + (palette().text_size + 2*padding),
      rect.width,
      rect.height - (palette().text_size + 2*padding)
    };
  }
  inline Rect interior_scrollbar() const
  {
    let interior = interior_rect();
    return {
      interior.x+interior.width - ScrollBarStyle::BUTTON_MIN_SIZE,
      interior.y,
      ScrollBarStyle::BUTTON_MIN_SIZE,
      interior.height 
    };
  }
  inline bool should_show_scrollbar()
  {
    return interior_total_height() > h() - (palette().text_size + 2*padding);
  }
  
  [[nodiscard]]
  inline bool opened() const
  { return rect.height > palette().text_size+2.f*padding+0.001f; }
  inline void open()
  {
    if (expand_height == VARIABLE_SIZE)
      rect.height = std::min(
        GetScreenHeight() - y(),
        interior_total_height() + (palette().text_size + padding*2)
      );
    else
      rect.height = expand_height;
    FOCUS_ME;
  }
  inline void close()
  { rect.height = palette().text_size+2.f*padding; }

  inline ItemList* pad(f32 p) { padding = p; return this; }

  void on_click(MouseButton) override;
  // These are only echoed to the scrollbar.
  void on_hover() override;
  void on_leave() override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_unfocus() override;
  void on_scroll() override;

  void update() override;
  void draw() override;

  ~ItemList() override;
};

