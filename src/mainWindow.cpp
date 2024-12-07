#include "include/mainWindow.h"
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

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent), scene(new visScene(this)), view(new visView(scene, this)) {

  // Set window minimum size.
  setMinimumSize(800, 600);

  // Set window title.
  setWindowTitle("VMC Workflow Modeler");

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
  connect(palette, &visPalette::signal_stackViewData, this, &MainWindow::slot_stackViewData);
  connect(palette, &visPalette::signal_nodeButtonClicked, view, &visView::slot_nodeButtonClicked);
  connect(scene, &visScene::signal_stackViewData, this, &MainWindow::slot_stackViewData);

  // Create a QTimer to periodically check the function and update the action's status
  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateActionStatus);
  timer->start(1000); // Check every second
}

MainWindow::~MainWindow() {
}

void MainWindow::updateActionStatus() {
  acNewWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
  acOpenWorkflow->setEnabled(!palette->isEmpty());
  acSaveWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
  acUndoWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty() && !viewDataStack.isEmpty());
  acGenerateScript->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
  acRunWorkflow->setEnabled(!palette->isEmpty() && !scene->items().isEmpty());
}

void MainWindow::createToolBar() {
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

  // Add actions to the bar.
  acOpenPalette = bar->addAction(QIcon(":/icons/open-palette.png"), "");
  connect(acOpenPalette, &QAction::triggered, this, &MainWindow::openPalette);
  acNewWorkflow = bar->addAction(QIcon(":/icons/new-workflow.png"), "");
  connect(acNewWorkflow, &QAction::triggered, this, &MainWindow::newWorkflow);
  acOpenWorkflow = bar->addAction(QIcon(":/icons/open-workflow.png"), "");
  connect(acOpenWorkflow, &QAction::triggered, this, &MainWindow::openWorkflow);
  acSaveWorkflow = bar->addAction(QIcon(":/icons/save-workflow.png"), "");
  connect(acSaveWorkflow, &QAction::triggered, this, &MainWindow::saveWorkflow);
  acUndoWorkflow = bar->addAction(QIcon(":/icons/undo-workflow.png"), "");
  connect(acUndoWorkflow, &QAction::triggered, this, &MainWindow::undoWorkflow);
  acGenerateScript = bar->addAction(QIcon(":/icons/generate-script.png"), "");
  connect(acGenerateScript, &QAction::triggered, this, &MainWindow::generateScript);
  acRunWorkflow = bar->addAction(QIcon(":/icons/run-workflow.png"), "");
  connect(acRunWorkflow, &QAction::triggered, this, &MainWindow::runWorkflow);

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
    "background-color: rgba(250, 226, 83, 1.0);"
    "}"
  );

  // Add bar to the layout.
  layout->addWidget(bar);

  // Set the layout for the container widget.
  toolbar->setLayout(layout);

  // Adjust the size of the container widget.
  toolbar->setFixedWidth(buttonSize * bar->actions().size());
  toolbar->setFixedHeight(buttonSize);

  // Move toolbar to the top of the MainWindow.
  toolbar->move(spacing - 3, spacing);
}

void MainWindow::createPalette() {
  // Create palette.
  palette = new visPalette(this);

  // Set palette position.
  palette->move(spacing, spacing * 3 + buttonSize);
}

void MainWindow::createInfoPane() {
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

void MainWindow::openPalette() { 
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

void MainWindow::newWorkflow() {
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

void MainWindow::openWorkflow() {
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

void MainWindow::saveWorkflow() {
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

void MainWindow::generateScript(bool silent) {
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
    QProcess::startDetached("notepad++.exe", QStringList() << pyFileName);
  }
}

QList<int> MainWindow::generateSequence(int rootIndex, const QList<visNode*>& nodes) {
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

QString MainWindow::generateCode(const QList<int>& sequence, const QList<visNode*>& nodes) {
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

void MainWindow::extractImports(QString& tplData, QStringList& imports) {
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

void MainWindow::replaceConnectedEditData(const visConnector* connector, QString& tplData) {
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

void MainWindow::replaceEditData(const QWidget* edit, QString& tplData) {
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

void MainWindow::replaceEditsData(const visNode* node, QString& tplData) {
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

void MainWindow::runWorkflow() {
  // Define popup menu.
  QMenu menu;
  QAction* action1 = menu.addAction("Send Workflow to Python");
  connect(action1, &QAction::triggered, this, &MainWindow::sendToPython);
  QAction* action2 = menu.addAction("Send Workflow to FormingSuite");
  connect(action2, &QAction::triggered, this, &MainWindow::sendToFormingSuite);
  QAction* action3 = menu.addAction("Send Workflow to Nexus.Compute");
  connect(action3, &QAction::triggered, this, &MainWindow::sendToNexusCompute);
  QAction* action = menu.exec(QCursor::pos());
  if (!action) {
    return;
  }
}

void MainWindow::sendToPython() {
  // Regenerate script file.
  generateScript(true);

  // Get palette dir.
  QString dir = palette->getDir();

  // Check if virtual environment exists.
  QString batFileName = QDir(dir).absoluteFilePath(".env/Scripts/activate.bat");
  if (QFileInfo(batFileName).exists()) {
    // Activate virtual environment and run Python script.
    qDebug() << "cmd.exe" << "/c" << batFileName << "&&" << "python" << pyFileName;
    QProcess::startDetached("cmd.exe", QStringList() << "/c" << batFileName << "&&" << "python" << pyFileName);
  }
  else {
    // Run Python script.
    qDebug() << "cmd.exe" << "/c" << "cd" << "python" << pyFileName;
    QProcess::startDetached("cmd.exe", QStringList() << "/c" << "python" << pyFileName);
  }
}

void MainWindow::sendToFormingSuite() {
  // Regenerate script file.
  generateScript(true);

  // Check value of the environment variable.
  QString dir = QDir(qgetenv("VMC_FS_PATH")).absolutePath();
  if (dir.isEmpty()) {
    qWarning("WARNING: Please set the VMC_FS_PATH environment variable.");
    QMessageBox::warning(this, "Warning", "Please set the VMC_FS_PATH environment variable.");
    return;
  }

  // Check RunScript.exe exists.
  QString batFileName = QDir(dir).absoluteFilePath("RunScript.bat");
  if (!QFileInfo(batFileName).exists()) {
    qWarning("WARNING: RunScript.bat not found.");
    QMessageBox::warning(this, "Warning", "RunScript.bat not found.");
    return;
  }

  // Start detached process to run the script.
  qDebug() << "cmd.exe" << "/c" << "cd" << dir << "&&" << batFileName << pyFileName;
  QProcess::startDetached("cmd.exe", QStringList() << "/c" << "cd" << dir << "&&" << batFileName << pyFileName);
}

void MainWindow::sendToNexusCompute() {
  // Regenerate script file.
  generateScript(true);

  // Get palette dir.
  QString dir = palette->getDir();

  // Check if virtual environment exists.
  QString batFileName = QDir(dir).absoluteFilePath(".env/Scripts/activate.bat");

  // Inform the user
  QMessageBox::information(nullptr, QStringLiteral("Nexus Compute"), QStringLiteral("Job monitor will open when the file upload is completed."));

  if (QFileInfo(batFileName).exists()) {
    QProcess* process = new QProcess(this);
    // Activate virtual environment and run Python script.
    qDebug() << "cmd.exe" << "/c" << batFileName << "&&" << "python" << pyFileName;

    connect(process, &QProcess::readyReadStandardOutput, [process, this]() {
        QString output = process->readAllStandardOutput();
        QString jobLink;

        for (const QString& log : output.split('\n')) {
            if (log.contains(QStringLiteral("https://"))) {
                jobLink = log;
                jobLink.replace(QStringLiteral("job submitted "), QStringLiteral("")).replace('\r', QStringLiteral("")).trimmed();
                qDebug() << "job link: " << jobLink;
                openLink(jobLink);
            }
        }
        // Read the output (for debugging)
        qDebug() << "Output:" << output;
    });
    connect(process, &QProcess::readyReadStandardError, [process]() {
        qDebug() << "Error:" << process->readAllStandardError();
    });
    process->start("cmd.exe", QStringList() << "/c" << batFileName << "&&" << "python" << pyFileName);
  }
  else {
    // Run Python script.
    qDebug() << "cmd.exe" << "/c" << "cd" << "python" << pyFileName;
    QProcess::startDetached("cmd.exe", QStringList() << "/c" << "python" << pyFileName);
  }
}

void MainWindow::slot_stackViewData() {
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

void MainWindow::slot_showInfo(const visNode* node) {
  QString info =
    "<style>"
    "h3 { font-size: 18px; margin: 10px 0; color: rgba(250, 226, 83, 1.0); }"
    "h4 { font-size: 16px; margin: 8px 0; color: rgba(0, 255, 0, 1.0); }"
    "p  { font-size: 14px; margin: 5px 0; color: white; }"
    "span  { font-size: 14px; font-style: italic; font-weight: bold; margin: 5px 0; color: white; }"
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

void MainWindow::undoWorkflow() {
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

void MainWindow::resizeEvent(QResizeEvent* event) {
  // Call the base class implementation
  QMainWindow::resizeEvent(event);

  // Get the new width and height.
  int newWidth = event->size().width();
  int newHeight = event->size().height();

  // Update the position of the infoPane.
  infoPane->move(newWidth - infoPaneWidth - spacing * 3, spacing + 3);
}

void MainWindow::openLink(QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}