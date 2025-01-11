#include "include/visMainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
  // Create application.
  QApplication a(argc, argv);

  // Set window icon.
  a.setWindowIcon(QIcon(":/icons/workflow.ico"));

  // Create main window.
  visMainWindow w;
  w.show();

  // Return application.
  return a.exec();
}
