#pragma once
#include <QGraphicsRectItem>
#include <QBrush>
#include <qgraphicsitem.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qvariant.h>
#include "common.hpp"

struct Clip : public QGraphicsRectItem
{

  Clip(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent)
    : QGraphicsRectItem(x, y, w, h),
      title_bar(0, 0, w, 0.2*h)
  {
    setFlags(
      ItemIsSelectable
    | ItemIsMovable
    | ItemIsFocusable
    | ItemSendsGeometryChanges
    );
    setBrush(QBrush(palette.dark()));
    title_bar.setBrush(QBrush(palette.accent()));
    title_bar.setParentItem(this);
    if (parent)
      setParentItem(parent);
  }

  Clip(const Clip&);

  Clip(Clip&&);

  Clip& operator = (const Clip& o);

  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

  QGraphicsRectItem title_bar;
};



