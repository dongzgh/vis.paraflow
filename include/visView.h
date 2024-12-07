#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include <QJsonObject>

class visView : public QGraphicsView {
  Q_OBJECT

public:
  visView(QGraphicsScene* scene, QWidget* parent = nullptr);
  void openFile(const QString& fileName);
  void deserialize(QByteArray& byteArray);
  void saveFile(const QString& fileName);
  void serialize(QByteArray& byteArray);

public slots:
  void slot_nodeButtonClicked(const QJsonObject& nodeDef);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

private:
  bool panning;
  QPoint lastMousePos;
};
