#include "split_proxy.hpp"


SplitProxy::SplitProxy(std::string&& name)
  : Hull(std::move(name))
{}

void SplitProxy::update()
{
  if (padding > 0)
    assert(!"SplitProxy cannot fit its child : that would defy its purpose.");
  if (child)
    child->update();
  Widget::update();
}
