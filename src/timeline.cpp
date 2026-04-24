#include "timeline.hpp"
#include "clipspace.hpp"
#include "common.hpp"
#include "grip_scroll_bar.hpp"
#include <qcoreevent.h>
#include <qevent.h>
#include <qgraphicsview.h>
#include <qgraphicswidget.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <QScrollBar>
#include <qscrollbar.h>
#include <qtransform.h>

TimelineScene::TimelineScene()
  : gui(new QGraphicsWidget()),
    scroll_anchor(),
    hscroll(),
    vscroll(),
    clip_space()
{
  setBackgroundBrush(palette().base());
  gui->setLayout(&scroll_anchor);
  auto hscroll_bar = new GripScrollBar();
  hscroll.setWidget(hscroll_bar);
  hscroll_bar->setOrientation(Qt::Horizontal);
  auto vscroll_bar = new GripScrollBar();
  vscroll.setWidget(vscroll_bar);
  vscroll_bar->setOrientation(Qt::Vertical);
  scroll_anchor.addCornerAnchors(
    &hscroll,
    Qt::TopRightCorner,
    &scroll_anchor,
    Qt::TopRightCorner
  );
  scroll_anchor.addCornerAnchors(
    &hscroll,
    Qt::TopLeftCorner,
    &scroll_anchor,
    Qt::TopLeftCorner
  );
  scroll_anchor.addCornerAnchors(
    &vscroll,
    Qt::BottomRightCorner,
    &scroll_anchor,
    Qt::BottomRightCorner
  );
  scroll_anchor.addAnchor(
    &vscroll,
    Qt::AnchorTop,
    &hscroll,
    Qt::AnchorBottom
  );
  addItem(gui.get());
  connect(
    hscroll_bar,
    &QScrollBar::valueChanged,
    this,
    &TimelineScene::scroll_update
  );
  connect(
    vscroll_bar,
    &QScrollBar::valueChanged,
    this,
    &TimelineScene::scroll_update
  );
}

bool TimelineScene::eventFilter(QObject *object, QEvent *event)
{
  if (object == &hscroll || object == &vscroll)
  {
    if (event->type() == QEvent::Enter)
    {
      return true;
    }
    // add other events here
  }
  return QGraphicsScene::eventFilter(object, event);
}

void TimelineScene::resizeEvent(QResizeEvent* event)
{
  const auto rect = size2rect(event->size());
  gui->setGeometry(rect);
  scroll_anchor.setGeometry(rect);
}

void TimelineScene::scroll_update(int value)
{
  const auto scr_h
    = qobject_cast<QScrollBar*>(hscroll.widget());
  const auto scr_v
    = qobject_cast<QScrollBar*>(vscroll.widget());
  const auto s0 = QPointF(
    float(scr_h->value())/scr_h->maximum(),
    float(scr_v->value())/scr_v->maximum()
  );
  const auto t0 = s0.x() * track_time;
  const auto xoffset = -t0 * current_cell_size() * zoom.x();
  const auto yoffset = -s0.y() * band_size * band_count * zoom.y();
  camera = QTransform(
    zoom.x(),        0, xoffset,
           0, zoom.y(), yoffset,
           0,        0,       1
  );
  clip_space.setTransform(camera);
}

Timeline::Timeline()
  : _scene(new TimelineScene()),
    _view(new QGraphicsView(_scene.get(), this))
{}

void Timeline::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  _scene->setSceneRect(
    0,
    0,
    event->size().width(),
    event->size().height()
  );
  _scene->resizeEvent(event);
}
