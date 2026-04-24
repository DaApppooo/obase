#include "grip_scroll_bar.hpp"
#include <qnamespace.h>
#include <qscrollbar.h>

GripScrollBar::GripScrollBar()
{
  setAttribute(Qt::WA_Hover);
}

bool GripScrollBar::event(QEvent* ev)
{
  return QScrollBar::event(ev);
}
