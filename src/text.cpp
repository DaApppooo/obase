#include "text.hpp"
#include "anchor.hpp"
#include "app.hpp"
#include "raylib.h"
#include <iostream>
using namespace std;

Text::Text(std::string&& txt)
  : Widget(std::string(txt)),
    _text(std::move(txt)),
    _rect_anchor(CENTER),
    select_start(1), select_end(0)
{}

void Text::on_click(MouseButton btn)
{
  if (not is_selectable())
    return;
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  cerr << "Text selection not supported for now." << endl;
}

void Text::on_drag(MouseButton btn)
{
  if (not is_selectable())
    return;
  if (btn != MOUSE_BUTTON_LEFT)
    return;
  cerr << "Text selection not supported for now." << endl;
}

void Text::on_double_click()
{
  if (not is_selectable())
    return;
  cerr << "Text selection not supported for now." << endl;
}

void Text::update() {}

void Text::draw()
{
  let measure = MeasureTextEx(font(), _text.c_str(), palette().text_size, SPACING);
  Vec2 pos;
  switch (_rect_anchor)
  {
    case LEFT:
      pos = { rect.x, rect.y + rect.height/2.f - measure.y/2.f };
      break;
    case RIGHT:
      pos = { rect.x+rect.width-measure.x,
              rect.y + rect.height/2.f - measure.y/2.f };
      break;
    case TOP:
      pos = { rect.x+rect.width/2.f-measure.x/2.f,
              rect.y };
      break;
    case BOTTOM:
      pos = { rect.x+rect.width/2.f-measure.x/2.f,
              rect.y + rect.height - measure.y };
      break;
    case CENTER_X:
    case CENTER_Y:
    case CENTER:
      pos = {
        rect.x + rect.width/2.f - measure.x/2.f,
        rect.y + rect.height/2.f - measure.y/2.f
      };
      break;
    case TOP_LEFT:
      pos = { rect.x, rect.y };
      break;
    case TOP_RIGHT:
      pos = { rect.x+rect.width-measure.x, rect.y };
      break;
    case BOTTOM_LEFT:
      pos = { rect.x, rect.y+rect.height-measure.y };
      break;
    case BOTTOM_RIGHT:
      pos = { rect.x+rect.width-measure.x, rect.y+rect.height-measure.y };
      break;
    default:
      assert(!"Unexpected branch.");
  }
  DrawTextEx(font(), _text.c_str(), pos, palette().text_size, SPACING, palette().text);
}

Text::~Text() {}

