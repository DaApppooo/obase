#pragma once
#include "widget.hpp"

enum AnchorPoint
{
  LEFT, RIGHT, TOP, BOTTOM,
  CENTER_X, CENTER_Y,

  TWO_POINT_ANCHOR_LIMIT,
  
  TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT,
  CENTER
};

struct Anchor
{
  Widget* A; // A is anchored
  AnchorPoint A_where;
  Widget* B; // on B
  AnchorPoint B_where : sizeof(AnchorPoint)*8-1;
  bool B_resize : 1;
  f32 _margin;

  Anchor()
    : A(nullptr), A_where(TWO_POINT_ANCHOR_LIMIT),
      B(nullptr), B_where(TWO_POINT_ANCHOR_LIMIT),
      B_resize(false), _margin(0)
  {}
  Anchor(const Anchor&) = default;

  inline Anchor& anchor(Widget* who, AnchorPoint where)
  { B = who; B_where = where; return *this; }
  inline Anchor& on(Widget* who, AnchorPoint where)
  { A = who; A_where = where; return *this; }
  inline Anchor& resizable() { B_resize = true; return *this; }
  inline Anchor& margin(f32 m) { _margin = m; return *this; }

  void update();
};


