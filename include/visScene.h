#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QDebug>

#include "include/visNode.h"
#include "include/visConnector.h"
#include "include/visLink.h"

class visScene : public QGraphicsScene {
  Q_OBJECT

public:
  enum MouseBehavior { None, Drag, Link };

public:
  visScene(QObject* parent = nullptr);
  QList<visNode*>& getNodes() { return nodes; }
  QList<visConnector*>& getConnectors() { return connectors; }  
  visNode* addNode(const QJsonObject& nodeDef, QPointF pos = QPointF(0, 0));
  visConnector* addConnector(visNode* startNode, int startIndex, visNode* endNode, int endIndex);
  visConnector* findConnector(const QString& id);
  visConnector* findInEditConnector(const QWidget* edit);
  visNode * rootNode();

signals:
  void signal_stackViewData();

public slots:
  void slot_update();

protected:
  void drawForeground(QPainter* painter, const QRectF& rect) override;
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
  visNode* nodePicked(const QPointF& pos) const;
  visConnector* connectorPicked(const QPointF& pos) const;
  void removeNode(visNode* node);
  void removeNodeFromNextNodes(visNode* node);
  QList<visConnector*> getNodeConnectors(visNode* node);
  void removeConnectorFromEdits(visConnector* connector);
  void removeEndNodeFromNextNodes(visConnector* connector);
  void removeConnector(visConnector* connector);
  void removeTitleConnector(const visNode * startNode, const visNode* endNode);
  void convertLinkToConnector(visLink* link);  

private:
  QList<visNode*> nodes;
  QList<visConnector*> connectors;
  MouseBehavior mouseBehavior = Drag;
  visLink* link = nullptr;
};
