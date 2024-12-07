#include "include/visScene.h"
#include "include/visPalette.h"
#include "include/visNode.h"
#include "include/visConnector.h"
#include "include/visLink.h"
#include "include/visFileSelector.h"
#include "include/mainWindow.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

visScene::visScene(QObject* parent)
  : QGraphicsScene(parent) {
  setSceneRect(0, 0, 10000, 10000);

  // Set scene background to dark gray.
  setBackgroundBrush(QColor(50, 50, 50));

  //// Set background texture.
  //QPixmap pixmap(":/textures/scene-grid.png");
  //setBackgroundBrush(pixmap);
}

visConnector* visScene::findConnector(const QString& id) {
  // Iterate all connectors.
  for (auto connector : connectors) {
    if (connector->id == id) {
      return connector;
    }
  }
  return nullptr;
}

visConnector* visScene::findInEditConnector(const QWidget* edit) {
  // Check if edit has connectors.
  QStringList connectorIds = edit->property("ConnectorIds").value<QStringList>();
  if (connectorIds.isEmpty()) {
    return nullptr;
  }
  // Iterate each connector.
  for (const QString& id : connectorIds) {
    // Find connector.
    visConnector* connector = findConnector(id);
    if (connector) {
      QWidget* endEdit = connector->getEndNode()->getEdits()[connector->getEndIndex()];
      if (endEdit == edit) {
        return connector;
      }
    }
  }
  // Return nullptr.
  return nullptr;
}

visNode* visScene::rootNode() {
  // Iterate all nodes.
  for (auto node : nodes) {
    // Check if the node exists in other nodes' nextNodes.
    bool exists = false;
    for (auto n : nodes) {
      if (n == node)
        continue;
      if (n->nextNodes.contains(node)) {
        exists = true;
        break;
      }
    }

    // If the node does not exist in other nodes' nextNodes, it is the root node.
    if (!exists) {
      return node;
    }
  }
  return nullptr;
}

void visScene::slot_update() {
  // Update all.
  update();
}

void visScene::drawForeground(QPainter* painter, const QRectF& rect) {
  // Draw the connectors.
  for (auto connector : connectors) {
    connector->paint(painter, nullptr, nullptr);
  }
}

visNode* visScene::addNode(const QJsonObject& nodeDef, QPointF pos) {
  // Create a new node.
  visNode* node = new visNode(nodeDef);

  // Add the node to the scene.
  addItem(node);

  // Connect the signal_nodeMoved signal to the update slot.
  connect(node, &visNode::signal_nodeMoved, this, &visScene::slot_update);

  // Connect the signal showInfo signal to the slot_showInfo slot.
  connect(node, &visNode::signal_showInfo, static_cast<MainWindow*>(parent()), &MainWindow::slot_showInfo);

  // Set a lower z-value for nodes.
  node->setZValue(0);

  // Position node.
  node->setPos(pos);

  // Add the node to the list of nodes.
  nodes.append(node);

  return node;
}

visConnector* visScene::addConnector(visNode* startNode, int startIndex, visNode* endNode, int endIndex) {
  // Create a new connector.
  visConnector* connector = new visConnector(startNode, startIndex, endNode, endIndex);

  // Add the connector to the scene.
  addItem(connector);

  // Set a higher z-value for connectors.
  connector->setZValue(1);

  // Add the connector to the list of connectors.
  connectors.append(connector);

  return connector;
}

void visScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  // Get the position of the mouse click.
  QPointF scenePos = event->scenePos();

  // Check if the mouse click are in the node's name bar area.
  for (auto node : nodes) {
    if (node->inTitleArea(scenePos)) {
      mouseBehavior = visScene::Drag;

      // Send signal_stackViewData signal.
      emit signal_stackViewData();
    }
  }

  // Check if the mouse click are in the node's input or output area.
  for (auto node : nodes) {
    int index = node->inHotArea(scenePos);
    if (index != -1) {
      // Create a link.
      mouseBehavior = visScene::Link;
      link = new visLink(scenePos, scenePos);
      addItem(link);

      // Set the start node of the link.
      link->startNode = node;

      // Set the start index of the link.
      link->startIndex = index;

      // Update the scene.
      update();
      return;
    }
  }

  QGraphicsScene::mousePressEvent(event);
}

void visScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
  // Get the position of the mouse click.
  QPointF scenePos = event->scenePos();

  if (mouseBehavior == visScene::Link) {
    // Check if the link is not null.
    if (link) {
      // Update the link.
      link->updateEndPosition(scenePos);

      // Update the scene.
      update();
      return;
    }
  }

  QGraphicsScene::mouseMoveEvent(event);
}

void visScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  // Get the position of the mouse click.
  QPointF scenePos = event->scenePos();

  // Check if mouseBehavior is Drag.
  if (mouseBehavior == visScene::Drag) {
    // Update all connector positions.
    for (auto connector : connectors) {
      connector->updatePosition();
    }

    // Reset action mode to None.
    mouseBehavior = visScene::None;

    // Update the scene.
    update();
  }

  // Check if mouseBehavior is Link.
  if (mouseBehavior == visScene::Link) { // Check if mouseBehavior is Link
    // Check if the link is not null.
    if (link) {
      // Update the link.
      link->updateEndPosition(scenePos);

      // Check if the link is near a node's input or output area.
      for (auto node : nodes) {
        int index = node->inHotArea(scenePos);
        if (index != -1) {
          // Set the end node of the link.
          link->endNode = node;

          // Set end index of the link.
          link->endIndex = index;
        }
      }

      // Conver to connector.
      convertLinkToConnector(link);

      // Remove the link.
      removeItem(link);
      delete link;
      link = nullptr;

      // Reset action mode to None.
      mouseBehavior = visScene::None;

      // Update the scene.
      update();
      return;
    }
  }

  // Call the base class implementation.
  QGraphicsScene::mouseReleaseEvent(event);
}

void visScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
  // Get the position of the mouse click.
  QPointF scenePos = event->scenePos();

  // Check if node is picked.
  if (visNode* node = nodePicked(scenePos)) {
    // Emit signal_stackViewData signal.
    emit signal_stackViewData();

    // Remove the node.
    removeNode(node);

    // Remove the node from nextNodes.
    removeNodeFromNextNodes(node);

    // Get the node connectors.
    QList<visConnector*> nodeConnectors = getNodeConnectors(node);

    // Delete the node connectors.
    for (auto connector : nodeConnectors) {
      removeConnector(connector);
      delete connector;
    }

    // Delete the node.
    delete node;

    // Update the scene.
    update();
  }
  else if (visConnector* connector = connectorPicked(scenePos)) {
    // Emit signal_stackViewData signal.
    emit signal_stackViewData();

    // Delete the connector.
    removeConnector(connector);
    delete connector;

    // Update the scene.
    update();
  }

  // Call the base class implementation.
  QGraphicsScene::mouseDoubleClickEvent(event);
}

visNode* visScene::nodePicked(const QPointF& pos) const {
  // Iterate all nodes.
  for (auto node : nodes) {
    if (node->inTitleArea(pos)) {
      return node;
    }
  }
  return nullptr;
}

visConnector* visScene::connectorPicked(const QPointF& pos) const {
  // Iterate all connectors.
  for (auto connector : connectors) {
    if (connector->inConnectorArea(pos)) {
      return connector;
    }
  }
  return nullptr;
}

void visScene::removeNode(visNode* node) {
  // Remove the node.
  removeItem(node);
  nodes.removeOne(node);
}

void visScene::removeNodeFromNextNodes(visNode* node) {
  // Iterate all nodes.
  for (auto n : nodes) {
    if (n == node)
      continue;

    // Remove the node from next nodes.
    n->nextNodes.removeOne(node);
  }
}

QList<visConnector*> visScene::getNodeConnectors(visNode* node) {
  // Prepare result.
  QList<visConnector*> result;

  // Iterate all connectors.
  for (auto connector : connectors) {
    // Check if the connector is connected to the node.
    if (connector->getStartNode() == node || connector->getEndNode() == node) {
      result.append(connector);
    }
  }

  // Return the result.
  return result;
}

void visScene::removeConnectorFromEdits(visConnector* connector) {
  // Get the start and end widgets.
  QWidget* startEdit = connector->getStartNode()->getEdits()[connector->getStartIndex()];
  QWidget* endEdit = connector->getEndNode()->getEdits()[connector->getEndIndex()];

  // Remove connector id from connected edits.
  startEdit->property("ConnectorIds").value<QStringList>().removeOne(connector->id);
  endEdit->property("ConnectorIds").value<QStringList>().removeOne(connector->id);
}

void visScene::removeEndNodeFromNextNodes(visConnector* connector) {
  // Iterate all connectors to find if there's still a connector started from start node and ended in end node.
  bool found = false;
  for (auto c : connectors) {
    if (c == connector)
      continue;
    // Check if the connector is connected to the end node.
    if (c->getStartNode() == connector->getStartNode() && c->getEndNode() == connector->getEndNode()) {
      found = true;
      break;
    }
  }

  // If not found, remove the end node from next nodes.
  if (!found) {
    connector->getStartNode()->nextNodes.removeOne(connector->getEndNode());
  }
}

void visScene::removeConnector(visConnector* connector) {
  // Remove the connector.
  removeItem(connector);
  connectors.removeOne(connector);

  // Remove connector id from connected edits.
  removeConnectorFromEdits(connector);

  // Remove the end node from next nodes.
  removeEndNodeFromNextNodes(connector);
}

void visScene::removeTitleConnector(const visNode* startNode, const visNode* endNode) {
  // Iterate all connectors to find if there's still a connector started from start node and ended in end node.
  bool found = false;
  visConnector* connector = nullptr;
  for (auto c : connectors) {
    // Check if the connector is connected to the end node.
    if (c->getStartNode() == startNode && c->getStartIndex() == 1 && c->getEndNode() == endNode && c->getEndIndex() == 0) {
      connector = c;
      found = true;
      break;
    }
  }

  // Remove the connector.
  if (found) {
    removeConnector(connector);
  }
}

void visScene::convertLinkToConnector(visLink* link) {
  // Check if link is empty.
  if (!link)
    return;

  // Check if start point and end point are the same.
  QPointF startPoint = link->line().p1();
  QPointF endPoint = link->line().p2();
  if (startPoint == endPoint)
    return;

  // Check if start node and end node are the same.
  visNode* startNode = link->startNode;
  visNode* endNode = link->endNode;
  if (startNode == endNode)
    return;

  // Check start and end index.
  if (link->startIndex == -1 || link->endIndex == -1)
    return;

  // Check if start edit and end edit are of the same edit type.
  QWidget* startEdit = startNode->getEdits()[link->startIndex];
  QWidget* endEdit = endNode->getEdits()[link->endIndex];
  if (startEdit->property("EditType") == endEdit->property("EditType"))
    return;

  // Check if start edit is an output edit.
  if (startEdit->property("EditType").toInt() != visNode::EditType::Out) {
    QWidget* tempEdit = startEdit;
    startEdit = endEdit;
    endEdit = tempEdit;
    startNode = link->endNode;
    endNode = link->startNode;
  }

  // Update link start and end index.
  int startIndex = startEdit->property("Index").toInt();
  int endIndex = endEdit->property("Index").toInt();

  // Validate if start node's nextNode already contains end node.
  if (startNode->nextNodes.contains(endNode) && startIndex == 1)
    return;

  // Remove the title connector.
  removeTitleConnector(startNode, endNode);

  // Add the connector.
  addConnector(startNode, startIndex, endNode, endIndex);
}
