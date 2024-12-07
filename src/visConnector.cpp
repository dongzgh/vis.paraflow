#include "include/visConnector.h"
#include "include/visNode.h"
#include "include/visUtilities.h"
#include <QPainter>
#include <QPainterPath>
#include <QWidget>

visConnector::visConnector(visNode* startNode, int startIndex, visNode* endNode, int endIndex)
  : startNode(startNode), startIndex(startIndex), endNode(endNode), endIndex(endIndex) {
  // Set id using GUID.
  id = visUtilities::randId();

  // Get the start and end widgets.
  startEdit = startNode->getEdits()[startIndex];
  endEdit = endNode->getEdits()[endIndex];

  // Add next nodes.
  if (startNode->nextNodes.indexOf(endNode) == -1)
    startNode->nextNodes.append(endNode);

  // Save connector to start and end edits.
  QStringList startEditConnectorIds = startEdit->property("ConnectorIds").value<QStringList>();
  startEditConnectorIds.append(id);
  startEdit->setProperty("ConnectorIds", startEditConnectorIds);
  QStringList endEditConnectorIds = endEdit->property("ConnectorIds").value<QStringList>();
  endEditConnectorIds.append(id);
  endEdit->setProperty("ConnectorIds", endEditConnectorIds);

  // Set slots for signal_nodeMoved signal.
  connect(startNode, &visNode::signal_nodeMoved, this, &visConnector::updatePosition);
  connect(endNode, &visNode::signal_nodeMoved, this, &visConnector::updatePosition);

  // Update the position.
  updatePosition();
}

void visConnector::updatePosition() {
  if (startIndex == -1 || endIndex == -1)
    return;

  // Get container position.
  QWidget* container = startEdit->parentWidget();
  QPointF containerPos = container ? container->pos() : QPointF(0, 0);

  // Get the start and end positions.
  QPointF startPos = startEdit->pos() + QPointF(startEdit->width() + startNode->getSpacing() / 2, startEdit->height() / 2) + containerPos;
  QPointF endPos = endEdit->pos() + QPointF(-endNode->getSpacing() / 2, endEdit->height() / 2) + containerPos;

  // Add space for hidden edits.
  if (!container) {
    startPos += QPointF(startNode->getSpacing(), 0);
    endPos += QPointF(endNode->getSpacing(), 0);
  }

  // Map the start and end positions to the scene.
  startPos = startNode->mapToScene(startPos);
  endPos = endNode->mapToScene(endPos);

  // Set the line.
  setLine(QLineF(startPos, endPos));
}

void visConnector::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  // Set the render hint to antialiasing.
  painter->setRenderHint(QPainter::Antialiasing, true);

  // Create a cubic bezier curve.
  QPainterPath path;
  QPointF startPoint = line().p1();
  QPointF endPoint = line().p2();
  QPointF controlPoint1;
  QPointF controlPoint2;
  if (startPoint.x() < endPoint.x()) {
    controlPoint1 = startPoint + QPointF((endPoint.x() - startPoint.x()) / 2, 0);
    controlPoint2 = endPoint + QPointF((startPoint.x() - endPoint.x()) / 2, 0);
  }
  else {
    controlPoint1 = startPoint + QPointF((startPoint.x() - endPoint.x()) / 2, 0);
    controlPoint2 = endPoint + QPointF((endPoint.x() - startPoint.x()) / 2, 0);
  }

  path.moveTo(startPoint);
  path.cubicTo(controlPoint1, controlPoint2, endPoint);

  // Create a linear gradient from start to end point.
  QLinearGradient gradient(startPoint, endPoint);
  gradient.setColorAt(0.0, startNode->getOutputColor());
  gradient.setColorAt(1.0, endNode->getInputColor());

  // Set the pen with the gradient.
  QPen pen(gradient, 2);
  painter->setPen(pen);
  painter->drawPath(path);
}

bool visConnector::inConnectorArea(const QPointF& pos) const {
  // Check if the pos is near the cubic bezier curve.
  QPainterPath path;
  QPointF startPoint = line().p1();
  QPointF endPoint = line().p2();
  QPointF controlPoint1 = startPoint + QPointF((endPoint.x() - startPoint.x()) / 2, 0);
  QPointF controlPoint2 = endPoint + QPointF((startPoint.x() - endPoint.x()) / 2, 0);

  // Create the cubic bezier curve.
  path.moveTo(startPoint);
  path.cubicTo(controlPoint1, controlPoint2, endPoint);

  // Create a stroked path with a tolerance.
  QPainterPathStroker stroker;
  stroker.setWidth(20); // Set the tolerance width
  QPainterPath strokedPath = stroker.createStroke(path);

  // Check if the pos is in the stroked path.
  return strokedPath.contains(pos);
}