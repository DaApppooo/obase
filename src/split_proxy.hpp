#pragma once
#include "hull.hpp"

/*

A split proxy is a widget that avoids propagating size changes any further.
This is useful with, for example, an ItemList, so that when it rescales when
it's opened, it doesn't affect the surrounding elements.

*/
struct SplitProxy : Hull
{
  SplitProxy(std::string&& name);
  
  void update() override;
};
