#include "include/visFileSelector.h"
#include <QHBoxLayout>
#include <QFileDialog>

visFileSelector::visFileSelector(QWidget* parent, bool isFolder) : QWidget(parent) {
  // Create the QLineEdit
  lineEdit = new QLineEdit(this);

  // Create the QPushButton
  button = new QPushButton(this);
  button->setText("..."); // Set button text or icon
  button->setFixedSize(lineEdit->height(), lineEdit->height()); // Make the button square and same height as QLineEdit

  // Create the layout and add widgets
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0); // Remove margins
  layout->setSpacing(0); // Remove spacing between widgets
  layout->addWidget(lineEdit);
  layout->addWidget(button);

  // Set the layout to the widget
  setLayout(layout);

  // Set line edit without border.
  lineEdit->setStyleSheet("QLineEdit { border: 0px; }");

  // Set button without border.
  button->setStyleSheet("QPushButton { border: 0px; }");

  // Set button height.
  button->setFixedHeight(lineEdit->height() / 2);

  // Connect button clicked signal to update lineEdit text.
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
  
  // Set the lineEdit text.
  if (!folderPath.isEmpty()) {
    lineEdit->setText(folderPath);
  }
}

void visFileSelector::updateFileText() {
  // Open a file dialog and set the selected file path to the lineEdit text.
  QString filePath = QFileDialog::getOpenFileName(nullptr, title, dir, filter);

  // Convert file path relative to the project path.
  filePath = QDir(QDir::currentPath()).relativeFilePath(filePath);

  // Set the lineEdit text.
  if (!filePath.isEmpty()) {
    lineEdit->setText(filePath);
  }
}

