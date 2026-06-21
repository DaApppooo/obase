#pragma once
#include <cctype>
#include <functional>
#include <string>
#include "anchor.hpp"
#include "app.hpp"
#include "push_button.hpp"
#include "raylib.h"
#include "raymath.h"

struct TextBoxStyle
{
  std::string text;
  std::function<bool(const std::string&, char)> filter;
  size_t cursor;
  size_t selection_start;
  BaseButtonStyle box_style;
  u16 keydown;
  u16 keydown_timer;
  f32 padding;

  TextBoxStyle()
    : text(),
      filter(nullptr),
      cursor(1),
      selection_start(1),
      box_style(),
      keydown(0), keydown_timer(1),
      padding(5.f)
  {}
  
  inline size_t clamp(size_t idx) { return std::clamp(idx, 0UL, text.size()); }
  inline size_t find_text_index(Rect total, Vec2 pos);
  inline void do_input(Widget* self, u16 input);
  /*
  Takes in a function which is ran over every utf-8 character of the internal
  text buffer, that should return true unless the loop can stop.
  - 'font' is the current font
  - 'codepoint' is the codepoint of the current character
  - 'glyph' is the glyph index of the current character
  - 'c' is the current character, fit inside a char32_t
  */
  inline void text_map(
    std::function<bool(
      Font& font,
      int codepoint,
      int glyph,
      size_t index,
      char32_t c,
      Vec2 size
    )>&&
  ) const;
  inline Vec2 measure_text() const;

  inline void on_hover();
  inline void on_leave();
  inline void on_click(Widget* self, MouseButton);
  inline void on_drag(Widget* self, MouseButton);
  inline void on_release(Widget* self, MouseButton);
  inline void on_keydown(Widget* self, u16 key);
  inline void on_keyup(Widget* self, u16 key);

  inline void draw(Widget* self, AnchorPoint align_y);
  inline void update(Widget* self);
};

#define ECHO_SETTERS(SELF) \
  inline SELF* set_text(std::string_view s) \
  { style.text.clear(); style.text += s; return this; } \
  inline SELF* set_filter( \
    std::function<bool(const std::string&, char)>&& new_filter \
  ) { \
    style.filter = std::move(new_filter); return this; \
  } \
  inline SELF* set_cursor_position(size_t pos) \
  { style.cursor = pos; return this; } \
  inline SELF* set_selection_start(size_t pos) \
  { style.selection_start = pos; return this; }

struct MonoTextBox : Widget
{
  std::function<void(const std::string&)> _on_validate;
  TextBoxStyle style;

  MonoTextBox(std::string&& name);

  ECHO_SETTERS(MonoTextBox);
  inline MonoTextBox* set_on_validate(
    std::function<void(const std::string&)> on_validate
  ) {
    _on_validate = std::move(on_validate);
    return this;
  }
  inline MonoTextBox* validate()
  { if (_on_validate) _on_validate(style.text); return this; }

  void on_hover() override;
  void on_leave() override;
  void on_click(MouseButton) override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_keydown(u16 key) override;
  void on_keyup(u16 key) override;

  void draw() override;
  void update() override;
};

struct MultiTextBox : Widget
{
  static macro u32 USER_RESIZABLE_W = 0b0000'0000'0000'0001;
  static macro u32 USER_RESIZABLE_H = 0b0000'0000'0000'0010;
  
  TextBoxStyle style;
  u32 flags;

  MultiTextBox(std::string&& name);
  
  ECHO_SETTERS(MultiTextBox);
  // The flags are declared inside MultiTextBox.
  inline MultiTextBox* set_flags(u32 flags)
  { this->flags = flags; return this; }
  
  void on_hover() override;
  void on_leave() override;
  void on_click(MouseButton) override;
  void on_drag(MouseButton) override;
  void on_release(MouseButton) override;
  void on_keydown(u16 key) override;
  void on_keyup(u16 key) override;

  void draw() override;
};

inline void TextBoxStyle::text_map(
  std::function<bool(
    Font& font,
    int codepoint,
    int glyph,
    size_t index,
    char32_t c,
    Vec2 size
  )>&& func
) const {
  Font& ft = font();
  Palette& pal = palette();
  let scale_factor = f32(pal.text_size) / ft.baseSize;
  let spacing = 2.f;
  size_t i = 0;
  for (auto& c : text)
  {
    int codepointByteCount = 0;
    int codepoint = GetCodepointNext(&c, &codepointByteCount);
    int index = GetGlyphIndex(ft, codepoint);
    Vec2 wh;
    if (c == '\n')
    {
      wh.x = 0.f;
      wh.y = pal.text_size;
    }
    else if (c == '\t')
    {
      wh.x = pal.dot_w * 4.f;
      wh.y = pal.text_size;
    }
    else
    {
      if (ft.glyphs[index].advanceX == 0)
        wh.x = ft.recs[index].width * scale_factor + spacing;
      else
        wh.x = ft.glyphs[index].advanceX * scale_factor + spacing;
      wh.y = ft.recs[index].height * scale_factor;
    }
    if (not func(ft, codepoint, index, i, c, wh))
      break;
    ++i;
  }
}

inline size_t TextBoxStyle::find_text_index(Rect total, Vec2 pos)
{
  let& pal = palette();
  // We move the bottom right until pos is in the rect.
  Vec2 br = {total.x + padding, total.y + pal.text_size + padding};
  if (br.x >= pos.x && br.y >= pos.y)
    return 0;
  size_t i = text.size();
  DrawLineV(br, pos, RED);
  text_map(
    [this, &i, &br, &pal, pos, total](Font&,
      int codepoint,
      int glyph,
      size_t index,
      char32_t c,
      Vec2 size
    ) -> bool {
      if (c == '\n')
      {
        br.x = total.x + padding;
        br.y += pal.text_size;
      }
      br.x += size.x;
      let surface = Rect{
        total.x,
        total.y,
        br.x - total.x,
        br.y - total.y
      };
      DrawRectangleLinesEx(surface, 1.f, RED);
      if (CheckCollisionPointRec(pos, surface))
      {
        i = index;
        return false;
      }
      return true;
    }
  );
  return i;
}

inline Vec2 TextBoxStyle::measure_text() const
{
  Palette& pal = palette();
  Vec2 max, cur;
  max = cur = {0.f, 0.f};
  text_map(
    [&max, &cur, &pal](
      Font&, int, int, size_t,
      char32_t c, Vec2 size
    ) -> bool {
      if (c == '\n')
      {
        cur.x = 0.f;
        cur.y += size.y;
      }
      else
      {
        cur.x += size.x;
      }
      if (cur.x > max.x)
        max.x = cur.x;
      if (cur.y > max.y)
        max.y = cur.y;
      return true;
    }
  );
  max.y += pal.text_size;
  return max;
}

inline void TextBoxStyle::on_hover()
{ box_style.on_hover(); }

inline void TextBoxStyle::on_leave()
{ box_style.on_leave(); }

inline void TextBoxStyle::on_click(Widget* self, MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  let mpo = GetMousePosition();
  if (not CheckCollisionPointRec(mpo, self->rect))
  {
    unfocus(self);
    app().redraw();
    return;
  }
  box_style.set_state(BTN_DOWN);
  cursor = selection_start = find_text_index(self->rect, mpo);
  focus(self);
}

inline void TextBoxStyle::on_drag(Widget* self, MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (box_style.state != BTN_DOWN)
    return;
  let mpo = GetMousePosition();
  cursor = find_text_index(self->rect, mpo);
  focus(self);
  app().redraw();
}

inline void TextBoxStyle::on_release(Widget* self, MouseButton btn)
{
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  if (box_style.state != BTN_DOWN)
    return;
  assert(is_focused(self));
  app().focused = nullptr; // avoid unfocusing
  box_style.on_release(self, nullptr, btn);
  app().focused = self;
}

inline void TextBoxStyle::do_input(Widget* self, u16 c)
{
  let SHIFT = IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT);
  let CTRL = IsKeyDown(KEY_LEFT_CONTROL) or IsKeyDown(KEY_RIGHT_CONTROL);
  println("INPUT: {} ('{}') SHIFT={}, CTRL={}", c, char(c), SHIFT, CTRL);
  if (CTRL)
  {
    switch (c)
    {
    case 'A':
      cursor = text.size();
      selection_start = 0;
      break;
    case 'C':
    {
      char* data = text.data();
      let left = std::min(cursor, selection_start);
      let right = std::max(cursor, selection_start);
      let temp = data[right];
      data[right] = 0;
      data += left;
      SetClipboardText(data);
      data -= left;
      data[right] = temp;
      break;
    }
    case 'V':
    {
      std::string_view str(GetClipboardText());
      let left = std::min(cursor, selection_start);
      let right = std::max(cursor, selection_start);
      text.replace_with_range(text.begin()+left, text.end()+right, str);
      break;
    }
    case KEY_LEFT:
      if (isalnum(text[cursor]))
      {
        while (isalnum(text[cursor]) and cursor > 0)
          --cursor;
      } else {
        while (not isalnum(text[cursor]) and cursor > 0)
          --cursor;
      }
      if (not SHIFT)
        selection_start = cursor;
      break;
    case KEY_RIGHT:
      if (isalnum(text[cursor]))
      {
        while (isalnum(text[cursor]) and cursor < text.size())
          ++cursor;
      } else {
        while (not isalnum(text[cursor]) and cursor < text.size())
          ++cursor;
      }
      if (not SHIFT)
        selection_start = cursor;
      break;
    case '\b':
    {
      if (cursor == 0 or not text.size())
        break;
      if (isalnum(text[cursor-1]))
      {
        while (cursor > 1 and isalnum(text[cursor-1]) or cursor == 1)
          --cursor;
      } else {
        while (cursor > 1 and not isalnum(text[cursor-1]) or cursor == 1)
          --cursor;
      }
      let min = std::min(selection_start, cursor);
      let max = std::max(selection_start, cursor);
      text.erase(
        text.begin() + min,
        text.begin() + max
      );
      selection_start = cursor;
      break;
    }
    case KEY_DELETE:
    {
      if (cursor >= text.size() or not text.size())
        break;
      if (isalnum(text[cursor]))
      {
        while (isalnum(text[cursor]) and cursor < text.size())
          ++cursor;
      } else {
        while (not isalnum(text[cursor]) and cursor < text.size())
          ++cursor;
      }
      let min = std::min(selection_start, cursor);
      let max = std::max(selection_start, cursor);
      text.erase(
        text.begin() + min,
        text.begin() + max
      );
      selection_start = cursor -= max - min;
      break;
    }
    }
    app().redraw();
    return;
  }
  if (c == '\r') // could be intercepted beforehand.
    text += '\n';
  else if ((c == '\b' or c == KEY_DELETE) and selection_start != cursor)
  {
    let min = std::min(cursor, selection_start);
    if (text.size())
    {
      text.erase(
        text.begin() + min,
        text.begin() + std::max(cursor, selection_start)
      );
      cursor = selection_start = min;
    }
  }
  else if (c == '\b')
  {
    if (text.size() and cursor > 0)
    {
      selection_start = --cursor;
      text.erase(text.begin() + cursor);
    }
  }
  else if (c == KEY_DELETE)
  {
    if (text.size() and cursor < text.size())
      text.erase(text.begin() + cursor);
  }
  else if (c == KEY_RIGHT)
  {
    if (SHIFT)
      ++cursor;
    else if (cursor != selection_start)
      selection_start = cursor = std::max(selection_start, cursor);
    else
      selection_start = ++cursor;
    cursor = std::clamp(cursor, 0UL, text.size());
    if (!SHIFT)
      selection_start = cursor;
  }
  else if (c == KEY_LEFT)
  {
    if (SHIFT)
    {
      if (cursor > 0)
        --cursor;
    }
    else if (cursor != selection_start)
      cursor = selection_start = std::min(selection_start, cursor);
    else if (cursor > 0)
      selection_start = --cursor;
    cursor = std::clamp(cursor, 0UL, text.size());
    if (not SHIFT)
      selection_start = cursor;
  }
  else if (isprint(c))
  {
    mut char_to_insert = static_cast<char>(c);
    if (not SHIFT)
      char_to_insert = std::tolower(char_to_insert);
    if (filter and not filter(text, char_to_insert))
      return;
    let min = clamp(std::min(cursor, selection_start));
    let max = clamp(std::max(cursor, selection_start));
    text.replace(
      text.begin() + min,
      text.begin() + max,
      1UL, char_to_insert
    );
    selection_start = cursor = std::clamp(min + 1, 0UL, text.size());
  }
  else
  {
    TraceLog(
      LOG_WARNING,
      "TextBox('%s') recieved non-printable input: %i",
      self->_name.c_str(),
      i32(c)
    );
  }
  app().redraw();
}

inline void TextBoxStyle::on_keydown(Widget* self, u16 key)
{
  assert(key);
  do_input(self, key);
  keydown = key;
  keydown_timer = 12;
}

inline void TextBoxStyle::on_keyup(Widget* self, u16 key)
{
  if (key == keydown)
    keydown = 0;
}

inline void TextBoxStyle::update(Widget* self)
{
  cursor = std::clamp(cursor, 0UL, text.size());
  selection_start = std::clamp(selection_start, 0UL, text.size());
  if (!keydown)
    return;
  if (keydown_timer == 0)
  {
    do_input(self, keydown);
  }
  else
    --keydown_timer;
}

inline void TextBoxStyle::draw(Widget* self, AnchorPoint align_y)
{
  assert(
     align_y == CENTER_Y
  or align_y == TOP
  or align_y == BOTTOM
  );
  let& pal = palette();
  DrawRoundedBox(
    self->rect,
    0.1f,
    0.1f,
    1.f,
    is_focused(self) ? pal.border(UI_SELECTED) : box_style.border(),
    box_style.bg()
  );
  if (box_style.update(GetFrameTime()))
    app().redraw();
  let m = measure_text();
  mut pos = Vec2{ self->rect.x + padding, 0};
  switch (align_y)
  {
  case CENTER_Y:
    pos.y = self->rect.y + padding
          + (self->rect.height - 2*padding) / 2.f - m.y / 2.f;
    break;
  case TOP:
    pos.y = self->rect.y + padding;
    break;
  case BOTTOM:
    pos.y = self->rect.y + padding
          + (self->rect.height - 2*padding) - m.y;
    break;
  default:
    [[assume(false)]];
    break;
  }
  let start = pos;
  let spacing = 2.f;
  char buf[2] = {0, 0};
  let scale_factor = f32(pal.text_size) / font().baseSize;
  let min = std::min(cursor, selection_start);
  let max = std::max(cursor, selection_start);
  text_map(
    [this, start, min, max, &pal, &pos](
      Font&, int codepoint, int glyph, size_t index, char32_t c, Vec2 size
    ) -> bool {
      let selected = min <= index and index < max;
      let prev = pos;
      if (c == '\n')
      {
        pos.x = start.x;
        pos.y += size.y;
      }
      else
        pos.x += size.x;
      if (selected)
      {
        DrawRectangleV(
          prev,
          {size.x, f32(pal.text_size)},
          pal.text_selected_bg
        );
      }
      if (text[index] != ' ')
      {
        DrawTextCodepoint(
          font(), codepoint, prev, pal.text_size,
          selected ? pal.text_selected_fg : pal.text
        );
      }
      if (index == cursor)
      {
        DrawLineV(
          prev,
          prev + Vec2{0.f, f32(pal.text_size)},
          pal.border(UI_ACTIVE)
        );
      }
      return true;
    }
  );
  if (cursor >= text.size())
  {
    DrawLineV(
      start + m - Vec2{0.f, f32(pal.text_size)},
      start + m,
      pal.border(UI_ACTIVE)
    );
  }
}

