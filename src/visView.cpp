#include "include/visScene.h"
#include "include/visView.h"
#include "include/visNode.h"
#include "include/visConnector.h"
#include "include/visFileSelector.h"
#include "include/visUtilities.h"
#include <QScrollBar>
#include <QFile>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

visView::visView(QGraphicsScene* scene, QWidget* parent)
  : QGraphicsView(scene, parent), panning(false) {
  // Get scene size.
  QRectF sceneRect = scene->sceneRect();
  
  // Center the view on the scene's center (0, 0).
  centerOn(sceneRect.width() / 2, sceneRect.height() / 2);
}

void visView::slot_nodeButtonClicked(const QJsonObject& nodeDef) {  
  // Get current view center.
  QPointF center = mapToScene(viewport()->rect().center());
  QPointF offset = QPointF(-150, -150);

  // Add node to the scene.
  dynamic_cast<visScene*>(scene())->addNode(nodeDef, center + offset);
}

void visView::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::MiddleButton) {
    // Start panning.
    panning = true;
    lastMousePos = event->pos();
  }
  else {
    QGraphicsView::mousePressEvent(event);
  }
}

void visView::mouseMoveEvent(QMouseEvent* event) {
  if (panning) {
    // Calculate the offset.
    QPoint offset = event->pos() - lastMousePos;
    lastMousePos = event->pos();

    // Move the view.
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - offset.x());
    verticalScrollBar()->setValue(verticalScrollBar()->value() - offset.y());
  }
  else {
    QGraphicsView::mouseMoveEvent(event);
  }
}

void visView::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::MiddleButton) {
    // Stop panning.
    panning = false;
  }
  else {
    QGraphicsView::mouseReleaseEvent(event);
  }  
}

void visView::wheelEvent(QWheelEvent* event) {
  // Zoom in or out.
  const double scaleFactor = 1.15;
  if (event->angleDelta().y() > 0) {
    // Zoom in.
    scale(scaleFactor, scaleFactor);
  }
  else {
    // Zoom out.
    scale(1.0 / scaleFactor, 1.0 / scaleFactor);
  }
}

void visView::openFile(const QString& fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("WARNING: Could not open file for reading.");
    QMessageBox::warning(this, "Warning", "Could not open file for reading.");
    return;
  }

  // Read the encrypted data from the file
  QByteArray encryptedData = file.readAll();
  file.close();

  // Decrypt the data using xorEncryptDecrypt
  QByteArray decryptedData = visUtilities::xorEncryptDecrypt(encryptedData, 'K'); // 'K' is the key used for XOR decryption

  // Deserialize the view.
  deserialize(decryptedData);
}

void visView::deserialize(QByteArray& byteArray) {
  // Create a data stream from the decrypted data
  QDataStream in(&byteArray, QIODevice::ReadOnly);
  in.setVersion(QDataStream::Qt_6_5);

  // Get scene.
  visScene* scene = dynamic_cast<visScene*>(this->scene());

  // Deserialize the number of nodes.
  int numNodes;
  in >> numNodes;

  // Deserialize each node.
  for (int i = 0; i < numNodes; ++i) {
    // Deserialize the node definition.
    QJsonObject nodeDef;
    in >> nodeDef;

    // Deserialize the node id.
    QString id;
    in >> id;

    // Deserialize the node pos.
    QPointF pos;
    in >> pos;

    // Add the node.
    visNode* node = scene->addNode(nodeDef, pos);

    // Set the node id.
    node->id = id;

    // Deserialize each edit.
    for (auto edit : node->getEdits()) {
      if (QComboBox* comboBox = qobject_cast<QComboBox*>(edit)) {
        // Deserialize the current text.
        QString currentText;
        in >> currentText;
        comboBox->setCurrentText(currentText);
      }
      else if (visFileSelector* fileSelector = qobject_cast<visFileSelector*>(edit)) {
        // Deserialize the text.
        QString text;
        in >> text;
        fileSelector->setText(text);
      }
      else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(edit)) {
        // Deserialize the text.
        QString text;
        in >> text;
        lineEdit->setText(text);
      }
    }
  }

  // Get scene nodes.
  QList<visNode*>& nodes = scene->getNodes();

  // Deserialize the number of connectors.
  int numConnectors;
  in >> numConnectors;

  // Deserialize each connector.
  for (int i = 0; i < numConnectors; ++i) {
    // Deserialize the connector.
    QString startNodeId;
    in >> startNodeId;

    // Deserialize the start index.
    int startIndex;
    in >> startIndex;

    // Deserialize the connector.
    QString endNodeId;
    in >> endNodeId;

    // Deserialize the end index.
    int endIndex;
    in >> endIndex;

    // Find the start and end nodes.
    int startNodeIndex = -1;
    int endNodeIndex = -1;
    for (int j = 0; j < nodes.size(); ++j) {
      if (nodes[j]->id == startNodeId) {
        startNodeIndex = j;
      }
      if (scene->getNodes()[j]->id == endNodeId) {
        endNodeIndex = j;
      }
    }

    // Add the connector.
    scene->addConnector(nodes[startNodeIndex], startIndex, nodes[endNodeIndex], endIndex);
  }

  // Deserialize the current view center.
  QPointF center;
  in >> center;
  centerOn(center);

  // Deserialize the current view scale.
  qreal scaleX;
  in >> scaleX;
  qreal scaleY;
  in >> scaleY;
  setTransform(QTransform::fromScale(scaleX, scaleY));
}

void visView::saveFile(const QString& fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("WARNING: Could not open file for writing.");
    QMessageBox::warning(this, "Warning", "Could not open file for writing.");
    return;
  }

  // Create a data stream from QByteArray.
  QByteArray byteArray;

  // Serialize the view.
  serialize(byteArray);

  // Encrypt the data using xorEncryptDecrypt
  QByteArray encryptedData = visUtilities::xorEncryptDecrypt(byteArray, 'K'); // 'K' is the key used for XOR encryption

  // Write the encrypted data to the file
  file.write(encryptedData);

  // Close the file.
  file.close();
}

void visView::serialize(QByteArray& byteArray) {
  // Create a data stream from QByteArray.
  QDataStream out(&byteArray, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_5);

  // Get scene nodes.
  QList<visNode*>& nodes = dynamic_cast<visScene*>(scene())->getNodes();

  // Serialize all nodes.
  out << (int)nodes.size();

  // Iterate over each node.
  for (auto node : nodes) {
    // Serialize node definition.
    out << node->getNodeDef();

    // Serialize node id.
    out << node->id;

    // Serialize node pos.
    out << node->pos();

    // Iterate over edits.
    for (auto edit : node->getEdits()) {
      if (QComboBox* comboBox = qobject_cast<QComboBox*>(edit)) {
        // Serialize the current text.
        out << comboBox->currentText();
      }
      else if (visFileSelector* fileSelector = qobject_cast<visFileSelector*>(edit)) {
        // Serialize the text.
        out << fileSelector->text();
      }
      else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(edit)) {
        // Serialize the text.
        out << lineEdit->text();
      }
    }
  }

  // Get scene connectors.
  QList<visConnector*>& connectors = dynamic_cast<visScene*>(scene())->getConnectors();

  // Serialize all connectors.
  out << (int)connectors.size();

  // Iterate over each connector.
  for (auto connector : connectors) {
    out << connector->getStartNode()->id;
    out << connector->getStartIndex();
    out << connector->getEndNode()->id;
    out << connector->getEndIndex();
  }

  // Get current view center.
  QPointF center = mapToScene(viewport()->rect().center());
  out << center;

  // Get current view scale.
  out << transform().m11();
  out << transform().m22();
}
