#pragma once

#include "include/visPalette.h"
#include "include/visView.h"
#include "include/visScene.h"
#include "include/visNode.h"
#include "include/visConnector.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>

class visMainWindow : public QMainWindow {
  Q_OBJECT

public:
  visMainWindow(QWidget* parent = nullptr);
  ~visMainWindow();

protected:
  void updateActionStatus();
  void createToolBar();
  void createPalette();
  void createInfoPane();
  void openPalette();
  void newWorkflow();
  void openWorkflow();
  void saveWorkflow();
  void undoWorkflow();
  void updateWorkflow();
  void generateScript(bool silent = false);
  void runWorkflow();
  void sendToPython();
  void resizeEvent(QResizeEvent* event) override;

public slots:
  void slot_stackViewData();
  void slot_showInfo(const visNode* node);

private:
  QList<int> generateSequence(int rootIndex, const QList<visNode*>& nodes);
  QString generateCode(const QList<int>& sequence, const QList<visNode*>& nodes);
  void extractImports(QString& tplData, QStringList& imports);
  void openLink(QString &url);
  void replaceConnectedEditData(const visConnector* connector, QString& tplData);
  void replaceEditData(const QWidget* edit, QString& tplData);
  void replaceEditsData(const visNode* node, QString& tplData);

private:
  int spacing = 10;
  int buttonSize = 40;
  int infoPaneWidth = 200;
  int infoPaneHeight = 400;
  QWidget* toolbar = nullptr;
  QLabel* infoPane = nullptr;
  QAction* acOpenPalette = nullptr;
  QAction* acNewWorkflow = nullptr;
  QAction* acOpenWorkflow = nullptr;
  QAction* acSaveWorkflow = nullptr;
  QAction* acUndoWorkflow = nullptr;
  QAction* acUpdateWorkflow = nullptr;
  QAction* acGenerateScript = nullptr;
  QAction* acRunWorkflow = nullptr;
  visPalette* palette = nullptr;
  visScene* scene = nullptr;
  QList<QByteArray> viewDataStack;
  visView* view = nullptr;
  QString wfFileName;
  QString pyFileName;
};
