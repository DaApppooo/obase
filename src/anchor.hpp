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

FORMATTER(AnchorPoint, [](std::ostream& out, const AnchorPoint& p)
{
  switch (p)
  {
#define ESTR(NAME) case NAME: out << #NAME; break;
    ESTR(LEFT) ESTR(RIGHT) ESTR(TOP) ESTR(BOTTOM)
    ESTR(CENTER_X) ESTR(CENTER_Y)
    ESTR(TOP_LEFT) ESTR(TOP_RIGHT) ESTR(BOTTOM_LEFT) ESTR(BOTTOM_RIGHT)
    ESTR(CENTER)
    default: out << "UNKNOWN"; break;
  }
});

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
  inline bool match(
    Widget* who, AnchorPoint where,
    Widget* anchored_to_who, AnchorPoint to_where
  ) const;

  // Returns true if the size of who's anchored changed.
  bool update();
};

inline bool Anchor::match(
  Widget* who, AnchorPoint where,
  Widget* anchored_to_who, AnchorPoint to_where
) const {
  return (
      who == B
  and where == B_where
  and anchored_to_who == A
  and to_where == A_where
  );
}

FORMATTER(Anchor, [](std::ostream& out, const Anchor& anch)
{
  out << "Anchor("
      << anch.B_where << " of " << anch.B
      << " anchored on "
      << anch.A_where << " of " << anch.A
      << ", resizable=" << anch.B_resize
      << ", margin=" << anch._margin << ")";
});

