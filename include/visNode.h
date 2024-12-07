#pragma once

#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QList>
#include <QPair>
#include <QObject>
#include <QJsonObject>
#include <QVBoxLayout>

class visNode : public QObject, public QGraphicsItem {
  Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
  enum EditType { In, Out };
  enum DataType { Int, Double, Bool, String, List, Object };

public:
  visNode(const QJsonObject& nodeDef);
  const QList<QWidget*>& getEdits() const { return edits; }
  const QColor& getInputColor() const { return inputColor; }
  const QColor& getOutputColor() const { return outputColor; }
  bool inTitleArea(const QPointF& pos) const;
  int inHotArea(const QPointF& pos) const;
  QJsonObject getNodeDef() const { return nodeDef; }
  int getSpacing() const { return spacing; }  

signals:
  void signal_nodeMoved();
  void signal_showInfo(const visNode* node);

protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;

private:
  void addTitleEdit(EditType editType);
  void addEdit(QJsonObject parameter);

public:
  QString id;
  QList<visNode*> nextNodes;
  
protected:
  QColor inputColor{ 70, 240, 3 };
  QColor outputColor{ 255, 1, 48 };

private:
  QJsonObject nodeDef;
  int titleHeight{ 30 };
  int editHeight{ 20 };
  int editWidth{ 200 };
  int spacing{ 10 };
  QWidget* container = nullptr;
  QVBoxLayout* layout = nullptr;
  QList<QWidget*> edits;
  QRectF titleBarRect;
  QList<QRectF> hotRects;
};
