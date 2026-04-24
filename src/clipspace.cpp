#include "clipspace.hpp"
#include "timeline.hpp"

void ClipSpace::add_clip(qreal x, qreal y)
{
    clips.append(Clip(
        x, y,
        qobject_cast<TimelineScene*>(scene())->current_cell_size(),
        TimelineScene::band_size,
        this
    ));
}

QPointF ClipSpace::screen_to_self(qreal x, qreal y)
{
return qobject_cast<TimelineScene*>(scene())->camera.map(QPointF(x,y));
}