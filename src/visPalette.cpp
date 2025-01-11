#include "include/visPalette.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QToolBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

visPalette::visPalette(QWidget* parent)
  : QWidget(parent), layout(new QVBoxLayout(this)), toolBox(new QToolBox()) {
  // Set background to be transparent.
  setAttribute(Qt::WA_TranslucentBackground);
  setStyleSheet("background-color: rgba(0, 0, 0, 0.5);");

  // Set layout.
  setLayout(layout);

  // Set layout margin.
  layout->setContentsMargins(0, 0, 0, 0);

  // Set layout spacing.
  layout->setSpacing(0);

  // Set layout alignment.
  layout->setAlignment(Qt::AlignLeft);

  // Set toolbox style.
  setToolBoxStyle();

  // Add the toolbox to the layout.
  layout->addWidget(toolBox);

  // Set dialog size.
  setFixedSize(width(), height());
}

visPalette::~visPalette() {
  delete layout;
}

bool visPalette::isEmpty() {
  return buttons.isEmpty();
}

int visPalette::width() {
  return columns * buttonSize + (columns + 1) * spacing;
}

int visPalette::height() {
  return rows * buttonSize + (rows + 1) * spacing;
}

// Add button click event.
void visPalette::buttonClicked() {
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  if (button) {
    // Get nodeDef from button.
    QJsonObject nodeDef = button->property("node").toJsonObject();

    // Emit signal_stackViewData signal.
    emit signal_stackViewData();

    // Emit signal_nodeButtonClicked signal with nodeDef.
    emit signal_nodeButtonClicked(nodeDef);
  }
}

void visPalette::setToolBoxStyle() {
  // Set toolbox style.
  toolBox->setStyleSheet(
    "QToolBox {"
    "background-color: rgba(0, 0, 0, 0.5);"
    "}"
    "QToolBox::tab {"
    "background-color: rgba(250, 226, 83, 1.0);"
    "color: black;"
    "text-transform: uppercase;"
    "font-weight: bold;"
    "}"
  );
}

void visPalette::clear() {
  // Clear buttons list.
  buttons.clear();
  
  // Delete everything in toolbox.
  if (toolBox) {
    delete toolBox;
    toolBox = new QToolBox();

    // Set toolbox style.
    setToolBoxStyle();

    // Add the toolbox to the layout.
    layout->addWidget(toolBox);
  }  
}

void visPalette::openFile(const QString& fileName) {
  // Open the JSON file.
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("WARNING: Could not open file for reading.");
    QMessageBox::warning(this, "Warning", "Could not open file for reading.");
    return;
  }

  // Get directory of the JSON file.
  dir = QFileInfo(fileName).absolutePath();

  // Set current dir to the JSON file directory.
  QDir::setCurrent(dir);

  // Read the JSON file and find 'include' array.
  QByteArray data = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  QJsonObject obj = doc.object();
  QJsonArray sections = obj["include"].toArray();

  // Iterate over each section.
  for (auto s : sections) {
    // Read section file.
    QString sectionFileName = QFileInfo(dir + "/" + s.toString()).absoluteFilePath();
    QFile sectionFile(sectionFileName);
    if (!sectionFile.open(QIODevice::ReadOnly)) {
      qWarning("WARNING: Could not open section file for reading.");
      QMessageBox::warning(this, "Warning", "Could not open section file for reading.");
      return;
    }

    // Read module name.
    QByteArray sectionData = sectionFile.readAll();
    QJsonDocument sectionDoc = QJsonDocument::fromJson(sectionData);
    QJsonObject sectionObj = sectionDoc.object();
    QString module = sectionObj["module"].toString();

    // Create section.
    QWidget* section = new QWidget();

    // Set section style.
    section->setStyleSheet(
      "QWidget {"
      "background-color: rgba(50, 50, 50, 0.5);"
      "}"
    );

    // Create section layout.
    QGridLayout* sectionLayout = new QGridLayout(section);
    section->setLayout(sectionLayout);

    // Set section layout margin.
    sectionLayout->setContentsMargins(10, 10, 10, 10);

    // Stretch each column and row of the grid layout.
    for (int i = 0; i < columns; i++) {
      sectionLayout->setColumnStretch(i, 1);
    }
    for (int i = 0; i < rows; i++) {
      sectionLayout->setRowStretch(i, 1);
    }
    sectionLayout->setSpacing(spacing);

    // Find nodes array in section data.
    QJsonArray nodes = sectionObj["nodes"].toArray();

    // Iterate each node object.
    int row = 0;
    int col = 0;
    for (auto node : nodes) {
      // Read node data.
      QJsonObject nodeDef = node.toObject();
      nodeDef["module"] = module;
      QString name = nodeDef["name"].toString();
      QString icon = nodeDef["icon"].toString();

      // Get icon path.
      QString iconPath = QFileInfo(dir + "/ico/" + module.toLower() + "/" + icon).absoluteFilePath();

      // Create node button.
      QPushButton* button = new QPushButton();      

      // Set button style.
      button->setStyleSheet(
        "QPushButton { "
        "background-color: rgba(0, 0, 0, 0.5); "
        "border: 0px;"
        "border-radius: 0px; "
        "} "
        "QPushButton:hover { "
        "background-color: rgba(0, 255, 0, 0.5); "
        "}"
      );

      // Set button icon.
      button->setIcon(QIcon(iconPath));
      button->setText("");

      // Scale button icon.
      button->setIconSize(QSize(buttonSize, buttonSize));

      // Set button hint to name.
      button->setToolTip(name);

      // Set button icon size.
      button->setFixedSize(buttonSize, buttonSize);

      // Save nodeDef to button.
      button->setProperty("node", nodeDef);

      // Connect button click event.
      connect(button, &QPushButton::clicked, this, &visPalette::buttonClicked);
      sectionLayout->addWidget(button, row, col++);

      // if col > columns - 1 then reset col to 0 and increment row. 
      if (col > columns - 1) {
        col = 0;
        row++;
      }

      // Add button to buttons list.
      buttons.append(button);
    }

    // Add section to toolbox.
    toolBox->addItem(section, module);
  }
}