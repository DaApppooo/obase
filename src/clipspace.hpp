#pragma once
#include "clip.hpp"
#include <QGraphicsWidget>
#include <QList>

struct ClipSpace : public QGraphicsWidget
{
  Q_OBJECT;
  
  void add_clip(qreal x, qreal y);

  QPointF screen_to_self(qreal x, qreal y);

  QList<Clip> clips;
};


