#pragma once

 #include <QColor>
 #include <QPixmap>
 #include <QSize>

 class visUtilities {
 public:
   static QPixmap colorSvgIcon(const QString& path, const QColor& color, const QSize& size);
   static QString randId();
   static QByteArray xorEncryptDecrypt(const QByteArray& data, char key);
 };
