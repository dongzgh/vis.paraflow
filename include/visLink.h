#pragma once

#include <QGraphicsLineItem>

class visNode;

class visLink : public QObject, public QGraphicsLineItem {
  Q_OBJECT

public:
  visLink(QPointF startPos, QPointF endPos);
  void updateEndPosition(QPointF endPos);

protected:
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

public:
  visNode* startNode = nullptr;
  visNode* endNode = nullptr;
  int startIndex = -1;
  int endIndex = -1;

private:
  QPointF startPos;
  QPointF endPos;
};
