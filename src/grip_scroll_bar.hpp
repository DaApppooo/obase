#pragma once
#include <QScrollBar>

struct GripScrollBar : QScrollBar
{
  Q_OBJECT;
  
  GripScrollBar();

  bool event(QEvent* ev) override;

  inline float zoom() { return float(value())/maximum(); }
};

