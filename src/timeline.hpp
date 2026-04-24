#pragma once
#include "clipspace.hpp"
#include <QGraphicsScene>
#include <QWidget>
#include <qevent.h>
#include <qgraphicsproxywidget.h>
#include <qgraphicsscene.h>
#include <qgraphicswidget.h>
#include <qpoint.h>
#include <qtmetamacros.h>
#include <qtransform.h>
#include <QGraphicsAnchor>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>

struct TimelineScene : public QGraphicsScene
{
  Q_OBJECT;

  static inline qreal min_cell_size = 25;
  static inline qreal band_size = 25;

  TimelineScene();

  qreal current_cell_size() const
  { return min_cell_size; } // scale this discreetely with zoom.x()
  
  bool eventFilter(QObject *object, QEvent *event) override;
  void resizeEvent(QResizeEvent* event);

  bad_ptr<QGraphicsWidget> gui;
  QGraphicsAnchorLayout scroll_anchor;
  QGraphicsProxyWidget hscroll;
  QGraphicsProxyWidget vscroll;
  ClipSpace clip_space;

  QPointF zoom{1, 1};
  QTransform camera;
  float track_time; ///< In seconds
  int band_count = 50;

  ~TimelineScene() {}

public slots:
  void scroll_update(int value);
};

struct Timeline : public QWidget
{
  Q_OBJECT;

  ptr<TimelineScene> _scene;
  ptr<QGraphicsView> _view;

  Timeline();

  void resizeEvent(QResizeEvent* event) override;
};


