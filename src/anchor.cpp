#include "anchor.hpp"

void Anchor::update()
{
  if (A_where < TWO_POINT_ANCHOR_LIMIT)
  {
    assert(B_where < TWO_POINT_ANCHOR_LIMIT);
    f32 v;
    switch (A_where)
    {
      case LEFT: v = A->left() + _margin; break;
      case RIGHT: v = A->right() - _margin; break;
      case TOP: v = A->top() + _margin; break;
      case BOTTOM: v = A->bottom() - _margin; break;
      case CENTER_X: v = A->x()+A->w()/2.f; break;
      case CENTER_Y: v = A->y()+A->h()/2.f; break;
      default:
        assert(!"Unexpected branch.");
    }
    const Rect old_B = B->rect;
    switch (B_where)
    {
      case LEFT:
        B->left(v);
        if (B_resize)
          B->w(B->w()+old_B.x-v);
        break;
      case RIGHT:
        if (B_resize)
          B->w(v - B->x());
        else
          B->right(v);
        break;
      case TOP:
        B->top(v);
        if (B_resize)
          B->h(B->h()+old_B.y-v);
        break;
      case BOTTOM:
        if (B_resize)
          B->h(v - B->y());
        else
          B->bottom(v);
        break;
      case CENTER_X:
        B->x(v - B->w()/2.f);
        if (B_resize)
          assert(!"Resizing with center anchor not supported.");
        break;
      case CENTER_Y:
        B->y(v - B->h()/2.f);
        if (B_resize)
          assert(!"Resizing with center anchor not supported.");
        break;
      default:
        assert(!"Unexpected branch.");
    }
  }
  else
  {
    assert(A_where > TWO_POINT_ANCHOR_LIMIT);
    assert(B_where > TWO_POINT_ANCHOR_LIMIT);
    Vec2 v;
    switch (A_where)
    {
      case TOP_LEFT: v = {A->left() + _margin, A->top() + _margin}; break;
      case TOP_RIGHT: v = {A->right() - _margin, A->top() + _margin}; break;
      case BOTTOM_LEFT: v = {A->left() + _margin, A->bottom() - _margin}; break;
      case BOTTOM_RIGHT: v = {A->right() - _margin, A->bottom() - _margin}; break;
      case CENTER: v = {A->x()+A->w()/2.f, A->y()+A->h()/2.f}; break;
      default:
        assert(!"Unexpected branch.");
    }
    const Rect old_B = B->rect;
    switch (B_where)
    {
      case TOP_LEFT:
        B->left(v.x);
        B->top(v.y);
        if (B_resize)
        {
          B->w(B->w()+old_B.x-v.x);
          B->h(B->h()+old_B.y-v.y);
        }
        break;
      case TOP_RIGHT:
        B->right(v.x);
        B->top(v.y);
        if (B_resize)
        {
          B->w(v.x - B->x());
          B->h(B->h()+old_B.y-v.y);
        }
        break;
      case BOTTOM_LEFT:
        B->left(v.x);
        B->bottom(v.y);
        if (B_resize)
        {
          B->w(B->w()+old_B.x-v.x);
          B->h(v.y - B->y());
        }
        break;
      case BOTTOM_RIGHT:
        B->right(v.x);
        B->bottom(v.y);
        if (B_resize)
        {
          B->w(v.x - B->x());
          B->h(v.y - B->y());
        }
        break;
      case CENTER:
        B->x(v.x - B->w()/2.f);
        B->y(v.y - B->h()/2.f);
        break;
      default:
        assert(!"Unexpected branch.");
    }
  }
}
