#include "include/mainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
  // Create application.
  QApplication a(argc, argv);

  // Set window icon.
  a.setWindowIcon(QIcon(":/icons/workflow.ico"));

  // Create main window.
  MainWindow w;
  w.show();

  // Return application.
  return a.exec();
}
