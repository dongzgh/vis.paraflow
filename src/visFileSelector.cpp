#include "include/visFileSelector.h"
#include "include/visNode.h"
#include <QHBoxLayout>
#include <QFileDialog>

visFileSelector::visFileSelector(QWidget* parent, bool isFolder) : QWidget(parent) {
  // Create the QLineEdit
  edit = new QLineEdit(this);

  // Set the width and height of the QLineEdit.
  edit->setMinimumWidth(visNode::editWidth - visNode::editHeight);
  edit->setMinimumHeight(visNode::editHeight);

  // Create the QPushButton
  button = new QPushButton(this);
  button->setText("..."); // Set button text or icon

  // Set button without border.
  button->setStyleSheet(
    "QPushButton {"
    "border: 0px;"
    "background-color: rgb(200, 200, 200)"
    "}"
  );

  // Set button width and height.
  button->setMinimumWidth(visNode::editHeight);
  button->setMinimumHeight(visNode::editHeight);

  // Create the layout and add widgets
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0); // Remove margins
  layout->setSpacing(0); // Remove spacing between widgets
  layout->addWidget(edit);
  layout->addWidget(button);

  // Set the layout to the widget
  setLayout(layout);

  // Set line edit without border.
  edit->setStyleSheet("QLineEdit { border: 0px; }");

  // Connect button clicked signal to update edit text.
  if (isFolder)
    connect(button, &QPushButton::clicked, this, &visFileSelector::updateFolderText);
  else
    connect(button, &QPushButton::clicked, this, &visFileSelector::updateFileText);
}

void visFileSelector::updateFolderText()
{
  QString folderPath = QFileDialog::getExistingDirectory(
    nullptr, QStringLiteral("Select Directory"), QStringLiteral(""),
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  // Convert folder path relative to the project path.
  folderPath = QDir::current().relativeFilePath(folderPath);
  
  // Set the edit text.
  if (!folderPath.isEmpty()) {
    edit->setText(folderPath);
  }
}

void visFileSelector::updateFileText() {
  // Open a file dialog and set the selected file path to the edit text.
  QString filePath = QFileDialog::getOpenFileName(nullptr, title, dir, filter);

  // Convert file path relative to the project path.
  filePath = QDir(QDir::currentPath()).relativeFilePath(filePath);

  // Set the edit text.
  if (!filePath.isEmpty()) {
    edit->setText(filePath);
  }
}

