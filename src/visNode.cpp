#include "include/visScene.h"
#include "include/visNode.h"
#include "include/visUtilities.h"
#include "include/visFileSelector.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetrics>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QCompleter>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QCoreApplication>
#include <QJsonArray>
#include <QComboBox>
#include <QPixmap>
#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolTip>
#include <QDir>

visNode::visNode(const QJsonObject& nodeDef) : nodeDef(nodeDef) {
  // Set id using GUID.
  id = visUtilities::randId();

  // Set hint for the graphics item.
  setToolTip(nodeDef["description"].toString());

  // Initialize node
  setFlag(ItemIsFocusable);
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);

  // Create a title bar.
  titleBarRect = QRect(spacing, 0, editWidth, titleHeight);
  QGraphicsRectItem * titleBar = new QGraphicsRectItem(titleBarRect, this);
  titleBar->setBrush(Qt::black);

  // Create a title text.
  QString module = nodeDef["module"].toString();
  QString name = nodeDef["name"].toString();
  QString title = module + " : " + name;
  QGraphicsTextItem* titleText = new QGraphicsTextItem(title, this);
  titleText->setDefaultTextColor(Qt::white);
  titleText->setPos(spacing, 0);

  // Create a container widget and layout.
  container = new QWidget();
  layout = new QVBoxLayout(container);  

  // Set the layout margins and spacing.
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(spacing);

  // Add hidden edits.
  addTitleEdit(EditType::In);
  addTitleEdit(EditType::Out);

  // Get node's parameters.
  QJsonArray parameters = nodeDef["parameters"].toArray();

  // Iterate each parameter.
  for (auto parameter : parameters) {
    addEdit(parameter.toObject());
  }

  // Set the layout for the container widget.
  container->setLayout(layout);

  // Add the container widget to the scene using a QGraphicsProxyWidget.  
  QGraphicsProxyWidget* containerProxy = new QGraphicsProxyWidget(this);
  containerProxy->setWidget(container);
  containerProxy->setPos(spacing, titleHeight + spacing);
}

void visNode::addTitleEdit(EditType editType) {
  // Create a QLineEdit.
  QWidget* edit = new QWidget();

  // Set edit box width.
  edit->setFixedWidth(editWidth);
  edit->setFixedHeight(editHeight);

  // Set data for the edit box.
  edit->setProperty("EditType", editType);
  edit->setProperty("Index", edits.size());
  QStringList connectorIds;
  edit->setProperty("ConnectorIds", QVariant::fromValue(connectorIds));

  // Add the edit box to the list.
  edits.append(edit);
}

void visNode::addEdit(QJsonObject parameter) {
  // Read parameter data.
  QString name = parameter["name"].toString();
  QString io = parameter["io"].toString();
  QString type = parameter["type"].toString();
  QVariant defaultValue;
  if (parameter.contains("default")) {
    defaultValue = parameter["default"].toVariant();
  }
  bool file = false;
  bool folder = false;
  QString title;
  QString filter;
  if (parameter.contains("folder"))
  { 
    folder = parameter["folder"].toBool();
  }
  else if (parameter.contains("file")) {
    file = parameter["file"].toBool();
    title = parameter["title"].toString();
    filter = parameter["filter"].toString();
  }
  QVariant options;
  if (parameter.contains("options")) {
    options = parameter["options"].toVariant();
  }

  // Convert io to visNode::EditType.  
  visNode::EditType editType = visNode::EditType::In;
  if (io == "in")
    editType = visNode::EditType::In;
  else if (io == "out")
    editType = visNode::EditType::Out;

  // Convert type to visNode::DataType.
  visNode::DataType dataType = visNode::DataType::String;
  if (type == "integer")
    dataType = visNode::DataType::Int;
  else if (type == "double")
    dataType = visNode::DataType::Double;
  else if (type == "bool")
    dataType = visNode::DataType::Bool;
  else if (type == "string")
    dataType = visNode::DataType::String;
  else if (type == "list")
    dataType = visNode::DataType::List; 
  else if (type == "object")
    dataType = visNode::DataType::Object;

  // If the parameter has options, create a QComboBox.
  if (!options.isNull()) {
    // Create a QComboBox.
    QComboBox* edit = new QComboBox();
    edit->setToolTip(name);

    // Add options to the QComboBox.
    for (const QVariant& option : options.toList()) {
      edit->addItem(option.toString());
    }

    // Set default value.
    if (!defaultValue.isNull()) {
      edit->setCurrentText(defaultValue.toString());
    }

    // Set combo box edit box width.
    edit->setMinimumWidth(editWidth);
    edit->setMinimumHeight(editHeight);

    // Set combo box without border.
    edit->setStyleSheet("QComboBox { border: 0px; }");

    // Set data for the edit box.
    edit->setProperty("EditType", editType);
    edit->setProperty("Name", name);
    edit->setProperty("DataType", dataType);
    edit->setProperty("Index", edits.size());
    QStringList connectorIds;
    edit->setProperty("ConnectorIds", QVariant::fromValue(connectorIds));

    // Add proxy widget to layout.
    layout->addWidget(edit);

    // Add the combo box to the list.
    edits.append(edit);
  }
  else if (file) {
    // Create a visFileSelector.
    visFileSelector* edit = new visFileSelector(nullptr);
    edit->setPlaceholderText(name);
    edit->setToolTip(name);

    // Set default value.
    if (!defaultValue.isNull()) {
      edit->setText(defaultValue.toString());
    }

    // Set edit box width and height.
    edit->setMinimumWidth(editWidth);
    edit->setMinimumHeight(editHeight);

    // Set edit title.
    edit->setTitle(title);
    edit->setDir(QDir::currentPath());
    edit->setFilter(filter);

    // Set data for the edit box.
    edit->setProperty("EditType", editType);
    edit->setProperty("Name", name);
    edit->setProperty("DataType", dataType);
    edit->setProperty("Index", edits.size());
    QStringList connectorIds;
    edit->setProperty("ConnectorIds", QVariant::fromValue(connectorIds));

    // Add proxy widget to layout.
    layout->addWidget(edit);

    // Add the edit box to the list.
    edits.append(edit);
  }
  else if (folder)
  {
    // Create a visFileSelector.
    visFileSelector* edit = new visFileSelector(nullptr, folder);
    edit->setPlaceholderText(name);
    edit->setToolTip(name);

    // Set default value.
    if (!defaultValue.isNull()) {
      edit->setText(defaultValue.toString());
    }

    // Set edit box width and height.
    edit->setMinimumWidth(editWidth);
    edit->setMinimumHeight(editHeight);

    // Set data for the edit box.
    edit->setProperty("EditType", editType);
    edit->setProperty("Name", name);
    edit->setProperty("DataType", dataType);
    edit->setProperty("Index", edits.size());
    QStringList connectorIds;
    edit->setProperty("ConnectorIds", QVariant::fromValue(connectorIds));

    // Add proxy widget to layout.
    layout->addWidget(edit);

    // Add the edit box to the list.
    edits.append(edit);
  }
  else
  {
    // Create a QLineEdit.
    QLineEdit* edit = new QLineEdit();
    edit->setPlaceholderText(name);
    edit->setToolTip(name);

    // Set validator based on data type.
    if (dataType == DataType::Int) {
      edit->setValidator(new QIntValidator());
    }
    else if (dataType == DataType::Double) {
      edit->setValidator(new QDoubleValidator());
    }

    // Set edit box without border.
    edit->setStyleSheet("QLineEdit { border: 0px; }");

    // Disable edit box if DataType is Object.
    if (dataType == DataType::Object) {
      edit->setReadOnly(true);

      // Set read-only style.
      edit->setStyleSheet(
        "QLineEdit {"
        "background-color: lightgray;"
        "color: black;"
        "border: 0px;"
        "}"
      );
    }

    // Disable edit box if it is an output.
    if (editType == EditType::Out) {
      edit->setReadOnly(true);

      // Set read-only style.
      edit->setStyleSheet(
        "QLineEdit {"
        "background-color: lightgray;"
        "color: black;"
        "border: 0px;"
        "}"
      );
    }

    // Set default value.
    if (!defaultValue.isNull()) {
      edit->setText(defaultValue.toString());
    }

    // Set edit box width.
    edit->setMinimumWidth(editWidth);
    edit->setMinimumHeight(editHeight);
    
    // Set data for the edit box.
    edit->setProperty("EditType", editType);
    edit->setProperty("Name", name);
    edit->setProperty("DataType", dataType);
    edit->setProperty("Index", edits.size());
    QStringList connectorIds;
    edit->setProperty("ConnectorIds", QVariant::fromValue(connectorIds));

    // Add proxy widget to layout.
    layout->addWidget(edit);

    // Add the edit box to the list.
    edits.append(edit);
  }

  return;
}

QRectF visNode::boundingRect() const { 
  return QRectF(0, 0, editWidth + spacing * 2, titleHeight + spacing + edits.size() * (editHeight + spacing));
}

void visNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  // Set the render hint to antialiasing.
  painter->setRenderHint(QPainter::Antialiasing, true);   
   
  // Set no pen.
  painter->setPen(Qt::NoPen);

  // Draw edit box hot areas.
  int yOffset = titleHeight + spacing;
  for (int i = 0; i < edits.size(); ++i) {
    if (i == 0) {
      // Draw for the in hidden edit box.
      QRectF inputHotRect(spacing / 2, 0, spacing / 2, titleHeight);      
      hotRects.append(inputHotRect);
      QRectF drawRect(spacing / 2, 0, editHeight, editHeight);
      painter->setBrush(inputColor);
      painter->drawRect(drawRect);
    }    
    else if (i == 1) {
      // Draw for the out hidden edit box.
      QRectF outputHotRect(boundingRect().width() - spacing, 0, spacing / 2, titleHeight);      
      hotRects.append(outputHotRect);
      QRectF drawRect(boundingRect().width() - spacing / 2 - editHeight, 0, editHeight, editHeight);
      painter->setBrush(outputColor);
      painter->drawRect(drawRect);
    }
    else
    {
      // Draw for the edit box.
      if (edits[i]->property("EditType").toInt() == EditType::In) {
        // Draw input hot area.
        QRectF inputHotRect(spacing / 2, yOffset, spacing / 2, editHeight);        
        hotRects.append(inputHotRect);
        QRectF drawRect(spacing / 2, yOffset, editHeight, editHeight);
        painter->setBrush(inputColor);
        painter->drawRect(drawRect);
      }
      else if (edits[i]->property("EditType").toInt() == EditType::Out) {
        // Draw output hot area.
        QRectF outputHotRect(boundingRect().width() - spacing, yOffset, spacing / 2, editHeight);        
        hotRects.append(outputHotRect);
        QRectF drawRect(boundingRect().width() - spacing / 2 - editHeight, yOffset, editHeight, editHeight);
        painter->setBrush(outputColor);
        painter->drawRect(drawRect);
      }

      // Increment y offset.
      yOffset += editHeight + spacing;
    }
  }
}

QVariant visNode::itemChange(GraphicsItemChange change, const QVariant& value) {
  if (change == ItemPositionChange && scene())
    emit signal_nodeMoved();
  return QGraphicsItem::itemChange(change, value);
}

bool visNode::inTitleArea(const QPointF& pos) const {
  return titleBarRect.contains(mapFromScene(pos));
}

int visNode::inHotArea(const QPointF& pos) const {
  int index = 0;
  for (const QRectF& rect : hotRects) {
    if (rect.contains(mapFromScene(pos)) && !inTitleArea(pos)) {
      return index;
    }
    ++index;
  }
  return -1;
}

void visNode::focusInEvent(QFocusEvent* event) {
  // Emit showInfo signal.
  emit signal_showInfo(this);

  // Call the base class implementation
  QGraphicsItem::focusInEvent(event);
}

void visNode::focusOutEvent(QFocusEvent* event) {
  // Emit showInfo signal.
  emit signal_showInfo(nullptr);

  // Call the base class implementation
  QGraphicsItem::focusOutEvent(event);
}