#pragma once

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QToolBox>

class visPalette : public QWidget {
  Q_OBJECT

public:
  visPalette(QWidget* parent = nullptr);
  ~visPalette();
  bool isEmpty();
  int width();
  int height();
  void clear();
  void openFile(const QString& fileName);
  QString getDir() { return dir; }

signals:
  void signal_stackViewData();
  void signal_nodeButtonClicked(const QJsonObject& nodeDef);

public slots:
  void buttonClicked();

private:
  void setToolBoxStyle();

private:
  QVBoxLayout* layout = nullptr;
  QToolBox* toolBox = nullptr;
  int columns = 4;
  int rows = 10;
  int buttonSize = 40;
  int spacing = 10;
  QString dir;
  QVector<QPushButton*> buttons;
};
