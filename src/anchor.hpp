#pragma once
#include "macros.hpp"
#include "types.hpp"
#include <ostream>

struct Widget;

enum AnchorPoint
{
  LEFT, RIGHT, TOP, BOTTOM,
  CENTER_X, CENTER_Y,

  TWO_POINT_ANCHOR_LIMIT,
  
  TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT,
  CENTER
};

macro std::ostream& format(std::ostream& out, const AnchorPoint& p)
{
  switch (p)
  {
#define C(NAME) case NAME: out << #NAME; break;
    C(LEFT) C(RIGHT) C(TOP) C(BOTTOM)
    C(CENTER_X) C(CENTER_Y)
    C(TOP_LEFT) C(TOP_RIGHT) C(BOTTOM_LEFT) C(BOTTOM_RIGHT)
    C(CENTER)
    default: out << "UNKNOWN"; break;
  }
  return out;
}

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

  // Returns true if the size of who's anchored changed.
  bool update();
};

macro std::ostream& format(std::ostream& out, const Anchor& anch)
{
  out << "Anchor("
      << anch.B_where << " of " << anch.B
      << anch.A_where << " of " << 
}

