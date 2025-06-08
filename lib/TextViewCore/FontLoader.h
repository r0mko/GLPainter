#pragma once

#include <QRawFont>

class FontLoader
{

public:
    FontLoader();

    QString fileName() const;
    void setFileName(const QString &fileName);
    bool isValid() const;

    qreal pixelSize() const;
    void setPixelSize(qreal pixelSize);

    int glyphIndex(QChar character) const;
    QRectF boundingRect(int glyphIndex) const;
    QString familyName() const;

    const QRawFont &font() const;
    QRawFont &font();

private:
    QString m_fileName;
    QRawFont m_font;
};
