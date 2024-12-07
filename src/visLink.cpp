#include "include/visLink.h"
#include "include/visNode.h"
#include <QPainter>
#include <QPainterPath>
#include <QWidget>

visLink::visLink(QPointF startPos, QPointF endPos)
  : startPos(startPos), endPos(endPos) {
  setLine(QLineF(startPos, endPos));
}

void visLink::updateEndPosition(QPointF endPos) {
  setLine(QLineF(startPos, endPos));
}

void visLink::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  // Set the render hint to antialiasing.
  painter->setRenderHint(QPainter::Antialiasing, true);

  // Create a cubic bezier curve.
  QPainterPath path;
  QPointF startPos = line().p1();
  QPointF endPos = line().p2();
  QPointF controlPoint1 = startPos + QPointF((endPos.x() - startPos.x()) / 2, 0);
  QPointF controlPoint2 = endPos + QPointF((startPos.x() - endPos.x()) / 2, 0);

  path.moveTo(startPos);
  path.cubicTo(controlPoint1, controlPoint2, endPos);

  // Set the pen with the gradient.
  QPen pen(QColor(255, 165, 0), 2);
  pen.setStyle(Qt::DashLine);
  painter->setPen(pen);
  painter->drawPath(path);
}
