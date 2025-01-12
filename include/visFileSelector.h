#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class visFileSelector : public QWidget {
  Q_OBJECT

public:
  visFileSelector(QWidget* parent = nullptr, bool isFolder = false);
  void setTitle(const QString& title) { this->title = title; };
  void setDir(const QString& dir) { this->dir = dir; };
  void setFilter(const QString& filter) { this->filter = filter; };
  void setPlaceholderText(const QString& text) { this->edit->setPlaceholderText(text); };
  QString text() { return edit->text(); };
  void setText(const QString& text) { edit->setText(text); };

private slots:
  void updateFileText();
  void updateFolderText();

private:
  QLineEdit* edit = nullptr;
  QPushButton* button = nullptr;
  QString title;
  QString dir;
  QString filter;
};
