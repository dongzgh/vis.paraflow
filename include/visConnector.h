#pragma once

#include <QGraphicsLineItem>

class visNode;

class visConnector : public QObject, public QGraphicsLineItem {
  Q_OBJECT

public:
  visConnector(visNode* startNode, int startIndex, visNode* endNode, int endIndex);
  void updatePosition();  
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
  bool inConnectorArea(const QPointF& pos) const;
  int getStartIndex() const { return startIndex;  };
  visNode* getStartNode() const { return startNode;  };
  int getEndIndex() const { return endIndex;  };
  visNode* getEndNode() const { return endNode;  };

public:
  QString id;

private:  
  visNode* startNode = nullptr; // Always assumed to be an output node.
  int startIndex = -1;
  QWidget* startEdit = nullptr;
  int startNodeIndex = -1;
  visNode* endNode; // Always assumed to be an input node.
  int endIndex = -1;
  QWidget* endEdit = nullptr;
  int endNodeIndex = -1;
};
