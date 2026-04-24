#include "clip.hpp"
#include "timeline.hpp"
#include <QVariant>
#include <QGraphicsItem>
#include <qapplication.h>
#include <qgraphicsitem.h>
#include <qobject.h>

Clip::Clip(const Clip& o)
  : QGraphicsRectItem(o.x(), o.y(),
    o.boundingRect().width(),
    o.boundingRect().height()
    ),
    title_bar(0, 0,
      o.boundingRect().width(),
      0.2*o.boundingRect().height()
    )
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
  if (o.parentItem())
    setParentItem(o.parentItem());
}

Clip::Clip(Clip&& o)
{
  force_move(o.title_bar, title_bar);
  force_move(o.d_ptr, d_ptr);
}

Clip& Clip::operator = (const Clip& o)
{
  std::construct_at(this, o);
  return *this;
}

QVariant Clip::itemChange(GraphicsItemChange ch, const QVariant& value)
{
  if (scene())
  {
    setRect(0, 0, rect().width(), TimelineScene::band_size);
  }
  if (ch == ItemPositionChange && scene())
  {
    auto new_pos = value.toPoint();
    if (QApplication::mouseButtons() == Qt::LeftButton)
    {
      auto tls = qobject_cast<TimelineScene*>(scene());
      const auto w = tls->current_cell_size();
      const auto h = TimelineScene::band_size;
      const auto x = round(new_pos.x()/w)*w;
      const auto y = round(new_pos.y()/h)*h;
      return QPointF(x, y);
    }
    else
      return new_pos;
  }
  else
    return QGraphicsItem::itemChange(ch, value);
}


