#pragma once
#include "macros.hpp"


struct BaseFader
{
  static macro f32 animation_time_scale = 10.f;
  // The animation will take 1 * animation_time_scale seconds to finish.
  f32 where;
  BaseFader()
    : where(1.f)
  {}

  void set() { where = 0.f; }
  
  // Returns true if the fader value has changed.
  bool update(float dt)
  {
    if (where == 1.f)
      return false;
    where += dt*animation_time_scale;
    if (where > 1.f)
      where = 1.f;
    return true;
  }
  
  template <class LinearType>
  inline LinearType get(const LinearType& a, const LinearType& b) const
  { return (1.f - where) * a + where * b; }
};

template <class T>
struct Fader
{
  T src, dst;
  BaseFader base;

  Fader(T init)
    : src(init), dst(init), base()
  {}

  void set(T new_target)
  {
    src = dst;
    dst = new_target;
    base.set();
  }

  inline T operator * () const
  { return base.get(src, dst); }
};


