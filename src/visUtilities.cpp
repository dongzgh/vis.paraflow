#include "include/visUtilities.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QUuid>
#include <QColor>
#include <QPixmap>
#include <QSize>
#include <QRandomGenerator>

QPixmap visUtilities::colorSvgIcon(const QString& path, const QColor& color, const QSize& size) {
  QSvgRenderer svgRenderer(path);
  QPixmap pixmap(size);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);

  // Scale the SVG to 80% of the button size.
  QSize scaledSize(size.width(), size.height());
  QRectF targetRect((size.width() - scaledSize.width()) / 2, (size.height() - scaledSize.height()) / 2, scaledSize.width(), scaledSize.height());
  svgRenderer.render(&painter, targetRect);

  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.fillRect(pixmap.rect(), color);
  painter.end();
  return pixmap;
}

QString visUtilities::randId() {
  // Create a random 3-character (only number) id.
  QString id="n";
  for (int i = 0; i < 3; i++) {
    id.append(QString::number(QRandomGenerator::global()->bounded(10)));
  }
  return id;
}

QByteArray visUtilities::xorEncryptDecrypt(const QByteArray& data, char key) {
  QByteArray result = data;
  for (int i = 0; i < result.size(); ++i) {
    result[i] = result[i] ^ key;
  }
  return result;
}
