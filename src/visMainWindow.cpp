#include "include/visMainWindow.h"
#include "include/visView.h"
#include "include/visScene.h"
#include "include/visNode.h"
#include "include/visConnector.h"
#include "include/visFileSelector.h"
#include <QWidget>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QQueue>
#include <QComboBox>
#include <QProcess>
#include <QRegularExpression>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QDockWidget>
#include <QLabel>
#include <QDesktopServices>

visMainWindow::visMainWindow(QWidget* parent)
  : QMainWindow(parent), scene(new visScene(this)), view(new visView(scene, this)) {

  // Set window minimum size.
  setMinimumSize(800, 600);

  // Set window title.
  setWindowTitle("Workflow Modeler");

  // Set window icon.
  setWindowIcon(QIcon(":/icons/workflow.png"));

  // Create toolbar.
  createToolBar();

  // Create palette.
  createPalette();

  // Create info pane.
  createInfoPane();

  // Set view widget size.
  setCentralWidget(view);

  // Get window size.
  int width = this->width();
  int height = this->height();

  // Connect signals and slots.
  connect(palette, &visPalette::signal_stackViewData, this, &visMainWindow::slot_stackViewData);
  connect(palette, &visPalette::signal_nodeButtonClicked, view, &visView::slot_nodeButtonClicked);
  connect(scene, &visScene::signal_stackViewData, this, &visMainWindow::slot_stackViewData);

  // Create a QTimer to periodically check the function and update the action's status
  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &visMainWindow::updateActionStatus);
  timer->start(1000); // Check every second
}

visMainWindow::~visMainWindow() {
}

void visMainWindow::updateActionStatus() {
  acNewWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
  acOpenWorkflow->setEnabled(!palette->isEmpty());
  acSaveWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
  acUndoWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty() && !viewDataStack.isEmpty());
  acGenerateScript->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
  acRunWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
}

void visMainWindow::createToolBar() {
  // Create a container widget.
  toolbar = new QWidget(this);

  // Create a vertical layout.
  QVBoxLayout* layout = new QVBoxLayout(toolbar);

  // Set layout margin.
  layout->setContentsMargins(0, 0, 0, 0);

  // Set layout spacing.
  layout->setSpacing(0);

  // Set layout alignment.
  layout->setAlignment(Qt::AlignLeft);

  // Create bar.
  QToolBar* bar = new QToolBar();

  // Set icon size for actions.
  bar->setIconSize(QSize(24, 24));

  // Add actions to the bar.
  acOpenPalette = bar->addAction(QIcon(":/icons/open-palette.png"), "");
  connect(acOpenPalette, &QAction::triggered, this, &visMainWindow::openPalette);
  acNewWorkflow = bar->addAction(QIcon(":/icons/new-workflow.png"), "");
  connect(acNewWorkflow, &QAction::triggered, this, &visMainWindow::newWorkflow);
  acOpenWorkflow = bar->addAction(QIcon(":/icons/open-workflow.png"), "");
  connect(acOpenWorkflow, &QAction::triggered, this, &visMainWindow::openWorkflow);
  acSaveWorkflow = bar->addAction(QIcon(":/icons/save-workflow.png"), "");
  connect(acSaveWorkflow, &QAction::triggered, this, &visMainWindow::saveWorkflow);
  acUndoWorkflow = bar->addAction(QIcon(":/icons/undo-workflow.png"), "");
  connect(acUndoWorkflow, &QAction::triggered, this, &visMainWindow::undoWorkflow);
  acGenerateScript = bar->addAction(QIcon(":/icons/generate-script.png"), "");
  connect(acGenerateScript, &QAction::triggered, this, &visMainWindow::generateScript);
  acRunWorkflow = bar->addAction(QIcon(":/icons/run-workflow.png"), "");
  connect(acRunWorkflow, &QAction::triggered, this, &visMainWindow::runWorkflow);

  // Set hints for actions.
  acOpenPalette->setToolTip("Open Palette");
  acNewWorkflow->setToolTip("New Workflow");
  acOpenWorkflow->setToolTip("Open Workflow");
  acSaveWorkflow->setToolTip("Save Workflow");
  acUndoWorkflow->setToolTip("Undo Workflow");
  acGenerateScript->setToolTip("Generate Script");
  acRunWorkflow->setToolTip("Run Workflow");

  // Only enable open-palette action at the beginning.
  acOpenPalette->setEnabled(true); 
  acNewWorkflow->setEnabled(false);
  acOpenWorkflow->setEnabled(false);
  acSaveWorkflow->setEnabled(false);
  acUndoWorkflow->setEnabled(false);
  acGenerateScript->setEnabled(false);
  acRunWorkflow->setEnabled(false);

  // Set bar size.
  bar->setFixedHeight(buttonSize);
  bar->setFixedWidth(buttonSize * bar->actions().size());

  // Set bar style.
  bar->setStyleSheet(
    "QToolBar {"
    "border: 2px;"
    "}"
    "QToolButton {"
    "background-color: rgba(0, 0, 0, 0.5);"
    "}"
    "QToolButton::hover {"
    "background-color: rgba(250, 226, 83, 0.5);"
    "}"
  );

  // Add bar to the layout.
  layout->addWidget(bar);

  // Set the layout for the container widget.
  toolbar->setLayout(layout);

  // Adjust the size of the container widget.
  toolbar->setFixedWidth(buttonSize * bar->actions().size());
  toolbar->setFixedHeight(buttonSize);

  // Move toolbar to the top of the visMainWindow.
  toolbar->move(spacing - 3, spacing);
}

void visMainWindow::createPalette() {
  // Create palette.
  palette = new visPalette(this);

  // Set palette position.
  palette->move(spacing, spacing * 3 + buttonSize);
}

void visMainWindow::createInfoPane() {
  // Create a container widget.
  infoPane = new QLabel("", this);

  // Set label alignment.
  infoPane->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  // Set label word wrap.
  infoPane->setWordWrap(true);

  // Set spacing.
  infoPane->setMargin(5);

  // Set the infoPane size.
  infoPane->setFixedWidth(infoPaneWidth);
  infoPane->setFixedHeight(infoPaneHeight);

  // Set <p> style in label.
  infoPane->setTextFormat(Qt::RichText);
}

void visMainWindow::openPalette() { 
  // Open palette file.
  QString fileName = QFileDialog::getOpenFileName(this, "Open Palette", "", "Palette Files (*.json)");
  if (!fileName.isEmpty()) {
    // Check if scene is not empty.
    if (!scene->items().isEmpty()) {
      newWorkflow();
    }

    // Clear the palette.
    palette->clear();

    // Open the palette file.
    palette->openFile(fileName);
  }
}

void visMainWindow::newWorkflow() {
  // Clear the scene.
  scene->clear();

  // Clear the nodes.
  QList<visNode*>& nodes = scene->getNodes();
  nodes.clear();

  // Clear the connectors.
  QList<visConnector*>& connectors = scene->getConnectors();
  connectors.clear();

  // Update the scene.
  scene->update();

  // Update the view.
  view->update();
}

void visMainWindow::openWorkflow() {
  // Open workflow file.
  QString fileName = QFileDialog::getOpenFileName(this, "Open Scene", "", "Scene Files (*.wf)");
  if (!fileName.isEmpty()) {
    // New a workflow.
    newWorkflow();

    // Open the workflow file.
    view->openFile(fileName);

    // Set workflow file name.
    wfFileName = fileName;
  }

  // Repaint window client area.
  QApplication::processEvents();
}

void visMainWindow::saveWorkflow() {
  // Save workflow file.
  QString fileName = QFileDialog::getSaveFileName(this, "Save Scene", "", "Scene Files (*.wf)");
  if (!fileName.isEmpty()) {
    view->saveFile(fileName);

    // Set workflow file name.
    wfFileName = fileName;
  }

  // Repaint window client area.
  QApplication::processEvents();
}

void visMainWindow::generateScript(bool silent) {
  // Get nodes.
  QList<visNode*>& nodes = scene->getNodes();

  // Get root node.
  visNode* rootNode = scene->rootNode();

  // Get root index.
  int rootIndex = nodes.indexOf(rootNode);

  // Generate sequence.
  QList<int> sequence = generateSequence(rootIndex, nodes);

  // Qdebug sequence.
  qDebug() << "Sequence:";
  for (int index : sequence) {
    qDebug() << nodes[index]->getNodeDef()["name"];
  }

  // Generate code.
  QString code = generateCode(sequence, nodes);

  // Get temp directory.
  QString tempDir = QDir::tempPath();

  // Set temp file name.
  pyFileName = tempDir + "/workflow.py";
  qDebug() << "==========================================";
  qDebug() << "Script File Name: " << pyFileName;
  qDebug() << "==========================================";

  // Write code to file.
  QFile file(pyFileName);
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream stream(&file);
    stream << code;
    file.close();
  }

  // Start detached process to open the script for editing.
  if (!silent) {
#ifdef _WIN32
    // Check if notepad++ exists.
    QString notepadPP = "C:/Program Files/Notepad++/notepad++.exe";
    if (QFileInfo(notepadPP).exists()) {
      QProcess::startDetached(notepadPP, QStringList() << pyFileName);
    }
    else {
      QProcess::startDetached("notepad", QStringList() << pyFileName);
    }
#else
    // Check if TextEdit exists.
    QString textEdit = "/System/Applications/TextEdit.app";
    if (QFileInfo(textEdit).exists()) {
      QProcess::startDetached("open", QStringList() << "-a" << textEdit << pyFileName);
    }
    else {  
      QProcess::startDetached("open", QStringList() << pyFileName);
    }
#endif
  }
}

QList<int> visMainWindow::generateSequence(int rootIndex, const QList<visNode*>& nodes) {
  QHash<visNode*, int> inDegree;
  QQueue<int> readyIndices;
  QList<int> sequence;

  // Initialize in-degree.
  for (visNode* node : nodes) {
    for (auto nextNode : node->nextNodes) {
      inDegree[nextNode]++;
    }
    if (!inDegree.contains(node)) {
      inDegree[node] = 0;
    }
  }

  // Start with the root index.
  readyIndices.enqueue(rootIndex);

  // Process indices in topological order.
  while (!readyIndices.isEmpty()) {
    int index = readyIndices.dequeue();
    sequence.append(index);

    visNode* node = nodes[index];
    for (auto nextNode : node->nextNodes) {
      inDegree[nextNode]--;
      if (inDegree[nextNode] == 0) {
        ;
        readyIndices.enqueue(nodes.indexOf(nextNode));
      }
    }
  }

  return sequence;
}

QString visMainWindow::generateCode(const QList<int>& sequence, const QList<visNode*>& nodes) {
  // Prepare return code.
  QString code;

  // Get palette directory.
  QString dir = palette->getDir();

  // Prepare imports.
  QStringList imports;

  // Prepare codes.
  QStringList codes;
  for (int index : sequence) {
    // Get node template.
    visNode* node = nodes[index];
    QString tpl = node->getNodeDef()["template"].toString();

    // Get module name.
    QString module = node->getNodeDef()["module"].toString();

    // Get template file name.
    QString tplFileName = QFileInfo(dir + "/tpl/" + module.toLower() + "/" + tpl).absoluteFilePath();

    // Get template file.
    QFile tplFile(tplFileName);
    if (!tplFile.open(QIODevice::ReadOnly)) {
      qWarning("WARNING: Could not open template file for reading.");
      QMessageBox::warning(this, "Warning", "Could not open template file for reading.");
      return code;
    }

    // Read template data.
    QString tplData = QString(tplFile.readAll());
    if (tplData.isEmpty()) {
      qWarning("WARNING: Template data is empty.");
      return code;
    }

    // Strip template data.
    tplData = tplData.trimmed();

    // Extract imports.
    extractImports(tplData, imports);

    // Replace template data.
    QString id = node->id;
    tplData = tplData.replace("node", id);

    // Repalce node edit data.
    replaceEditsData(node, tplData);

    // Append template data to codes.
    codes.append(tplData);
  }

  // Convert codes to a single string.
  code = codes.join("\n\n");

  // Add imports to the code.
  if (!imports.isEmpty())
  {
    QString header = imports.join("\n");
    code = header + "\n\n" + code;
  }

  // Return the code.
  return code;
}

void visMainWindow::extractImports(QString& tplData, QStringList& imports) {
  // Iterate each line in template data.
  QStringList lines = tplData.split("\n");
  QStringList newLines;
  for (int i = 0; i < lines.size(); ++i) {
    // Get line.
    QString line = lines[i];
    // Check if line is import.
    if (line.contains("import ")) {
      // Check if import is already in imports.
      if (imports.indexOf(line.trimmed()) != -1) {
        continue;
      }
      // Add import to imports.
      imports.append(line.trimmed());
    }
    else {
      newLines.append(line);
    }
  }

  // Join new lines.
  tplData = newLines.join("\n");

  // Trim template data.
  tplData = tplData.trimmed();
}

void visMainWindow::replaceConnectedEditData(const visConnector* connector, QString& tplData) {
  // Get start node of the connector.
  visNode* startNode = connector->getStartNode();

  // Get start node index.
  int startIndex = connector->getStartIndex();

  // Get start edit.
  QWidget* startEdit = startNode->getEdits()[startIndex];

  // Get start node id.
  QString startNodeId = startNode->id;

  // Get start edit name.
  QString startEditName = startEdit->property("Name").toString();

  // Convert start edit name to lower case.
  startEditName = startEditName.toLower();

  // Replace start edit name space with underscore.
  startEditName = startEditName.replace(" ", "_");

  // Compose the variable name.
  QString outVarName = startNodeId + "_out_" + startEditName;

  // Get end node of the connector.
  visNode* endNode = connector->getEndNode();

  // Get end node index.
  int endIndex = connector->getEndIndex();

  // Get end edit.
  QWidget* endEdit = endNode->getEdits()[endIndex];

  // Get end node id.
  QString endNodeId = endNode->id;

  // Get end edit name.
  QString endEditName = endEdit->property("Name").toString();

  // Convert end edit name to lower case.
  endEditName = endEditName.toLower();

  // Replace end edit name space with underscore.
  endEditName = endEditName.replace(" ", "_");

  // Compose the variable name.
  QString inVarName = "in_" + endEditName;

  // Replace in variable name with out variable name in code.
  tplData = tplData.replace(QRegularExpression("\\b" + inVarName + "\\b"), outVarName);
}

void visMainWindow::replaceEditData(const QWidget* edit, QString& tplData) {
  // Get edit data type.
  visNode::DataType dataType = (visNode::DataType)edit->property("DataType").toInt();
  if (dataType == visNode::DataType::Object) {
    return;
  }

  // Get edit type.
  visNode::EditType editType = (visNode::EditType)edit->property("EditType").toInt();

  // Convert edit type to string.
  QString editTypeStr;
  switch (editType) {
  case visNode::EditType::In:
    editTypeStr = "in";
    break;
  case visNode::EditType::Out:
    editTypeStr = "out";
    break;
  }

  // Get edit name.
  QString editName = edit->property("Name").toString();

  // Convert edit name to lower case.
  editName = editName.toLower();

  // Replace edit name space with underscore.
  editName.replace(" ", "_");

  // Compose the variable name.
  QString varName = editTypeStr + "_" + editName;

  // Check if edit is QLineEidt or QComboBox.
  QString editData;
  if (QComboBox* comboBox = qobject_cast<QComboBox*>(const_cast<QWidget*>(edit))) {
    // Get edit data.
    editData = comboBox->currentText();
  }
  else if (visFileSelector* fileSelector = qobject_cast<visFileSelector*>(const_cast<QWidget*>(edit))) {
    // Get edit data.
    editData = fileSelector->text();

    // Convert relative path to absolute path.
    if (!editData.isEmpty() && !editData.startsWith("/")) {
      editData = QDir::current().absoluteFilePath(editData);
    }
  }
  else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(const_cast<QWidget*>(edit))) {
    // Get edit data.
    editData = lineEdit->text();
  }

  // Check if edit data is boolean.
  if (dataType == visNode::DataType::Bool) {
    // Convert edit data to boolean.
    editData = editData == "true" ? "True" : "False";
  }

  // Replace edit data, only whole word.
  tplData = tplData.replace(QRegularExpression("\\b" + varName + "\\b"), editData);
}

void visMainWindow::replaceEditsData(const visNode* node, QString& tplData) {
  // Get node edits.
  QList<QWidget*> edits = node->getEdits();

  // Iterate each edit.
  int index = 0;
  for (QWidget* edit : edits) {
    if (index == 0 || index == 1) {
      index++;
      continue;
    }
    // Find in edit connector.
    visConnector* connector = scene->findInEditConnector(edit);
    if (connector) {
      replaceConnectedEditData(connector, tplData);
    }
    else {
      replaceEditData(edit, tplData);
    }
    index++;
  }
}

void visMainWindow::runWorkflow() {
  // Define popup menu.
  QMenu menu;
  QAction* action1 = menu.addAction("Send Workflow to Python");
  connect(action1, &QAction::triggered, this, &visMainWindow::sendToPython);
  QAction* action = menu.exec(QCursor::pos());
  if (!action) {
    return;
  }
}

void visMainWindow::sendToPython() {
  // Regenerate script file.
  generateScript(true);

  // Get palette dir.
  QString dir = palette->getDir();

  // Start detached process to run the script.
  QProcess process;
  process.setWorkingDirectory(dir);

#ifdef _WIN32
  // Check if virtual environment exists.
  QString batFileName = QDir(dir).absoluteFilePath(".env/Scripts/activate.bat");
  if (QFileInfo(batFileName).exists()) {
    // Activate virtual environment and run Python script.
    QString command = batFileName + " && " + "python " + pyFileName;
    qDebug() << "cmd.exe" << "/c" << command;
    process.start("cmd.exe", QStringList() << "/c" << command);
  }
  else {
    // Run Python script.
    QString command = "python " + pyFileName;
    qDebug() << "cmd.exe /c " << command;
    process.start("cmd.exe", QStringList() << "/c" << command);
  }
#else
  // Check if virtual environment exists.
  QString shFileName = QDir(dir).absoluteFilePath(".env/bin/activate");
  if (QFileInfo(shFileName).exists()) {
    // Activate virtual environment and run Python script.
    QString command = "source " + shFileName + " && " + "python " + pyFileName;
    qDebug() << "bash -c " << command;
    process.start("bash", QStringList() << "-c" << command);
  }
  else {
    // Run Python script.
    QString command = "python " + pyFileName;
    qDebug() << command;
    process.start("python", QStringList() << command);
  }
#endif

  // Wait for the process to finish.
  process.waitForFinished();
}

void visMainWindow::slot_stackViewData() {
  // Serialize the current view.
  QByteArray viewData;
  view->serialize(viewData);

  // Get the last view data.
  QByteArray lastViewData;
  if (!viewDataStack.empty()) {
    lastViewData = viewDataStack.back();
  }

  // Push the copy scene.
  if (viewData != lastViewData)
    viewDataStack.push_back(viewData);
}

void visMainWindow::slot_showInfo(const visNode* node) {
  QString info =
    "<style>"
    "h3 { font-size: 18px; margin: 10px 0; color: rgb(250, 226, 83); }"
    "h4 { font-size: 16px; margin: 8px 0; color: rgb(0, 255, 0); }"
    "p  { font-size: 14px; margin: 5px 0; color: rgb(255, 255, 255); }"
    "span  { font-size: 14px; font-style: italic; font-weight: bold; margin: 5px 0; color: rgb(255, 255, 255); }"
    "</style>";

  // Check if node is null.
  if (!node) {
    infoPane->setText(info);
    return;
  }
  else {
    // Set the info pane text.
    info += "<h3>" + node->getNodeDef()["name"].toString() + "</h3>";
    info += "<p>" + node->getNodeDef()["description"].toString() + "</p>";  

    // Iterate each edit.
    QList<QWidget*> edits = node->getEdits();

    // Check the number of edits.
    if (edits.size() == 2) {
      infoPane->setText(info);
      return;
    }
    else {
      // Append parameters to info.
      info += "<h3>" + QString("Parameters") + "</h3>";

      // Iterate each edit.
      for (QWidget* edit : edits) {
        // Skip the first two edits.
        if (edits.indexOf(edit) == 0 || edits.indexOf(edit) == 1) {
          continue;
        }

        // Get edit name.
        QString editName = edit->property("Name").toString();

        // Get edit data type.
        visNode::DataType dataType = (visNode::DataType)edit->property("DataType").toInt();

        // Get edit type.
        visNode::EditType editType = (visNode::EditType)edit->property("EditType").toInt();

        // Convert edit type to string.
        QString editTypeStr;
        switch (editType) {
        case visNode::EditType::In:
          editTypeStr = "Input";
          break;
        case visNode::EditType::Out:
          editTypeStr = "Output";
          break;
        }
        // Convert data type to string.
        QString dataTypeStr;
        switch (dataType) {
        case visNode::DataType::Int:
          dataTypeStr = "Integer";
          break;
        case visNode::DataType::Double:
          dataTypeStr = "Double";
          break;
        case visNode::DataType::Bool:
          dataTypeStr = "Boolean";
          break;
        case visNode::DataType::String:
          dataTypeStr = "String";
          break;
        case visNode::DataType::List:
          dataTypeStr = "List";
          break;
        case visNode::DataType::Object:
          dataTypeStr = "Object";
          break;
        }
        // Append edit name, edit type and data type to info.
        info += "<h4>" + editName + "</h4>";
        info += "<p>" + QString("<span>io:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>") + editTypeStr + "</p>";
        info += "<p>" + QString("<span>type:&nbsp;&nbsp;</span>") + dataTypeStr + "</p>";
      }
      infoPane->setText(info);

      // Update infoPane height according to the text length.
      int height = infoPane->sizeHint().height();
      infoPane->setFixedHeight(height);
    }    
  }
}

void visMainWindow::undoWorkflow() {
  // New a workflow.
  newWorkflow();

  // Deserialize the scene data.
  QByteArray viewData = viewDataStack.back();
  view->deserialize(viewData);

  // Pop the scene data stack.
  viewDataStack.pop_back();

  // Repaint window client area.
  QApplication::processEvents();
}

void visMainWindow::resizeEvent(QResizeEvent* event) {
  // Call the base class implementation
  QMainWindow::resizeEvent(event);

  // Get the new width and height.
  int newWidth = event->size().width();
  int newHeight = event->size().height();

  // Update the position of the infoPane.
  infoPane->move(newWidth - infoPaneWidth - spacing * 3, spacing + 3);
}

void visMainWindow::openLink(QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}
