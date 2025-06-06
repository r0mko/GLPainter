#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QRawFont>

class FontLoader : public QObject
{
    QML_ELEMENT
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)
    Q_PROPERTY(qreal pixelSize READ pixelSize WRITE setPixelSize NOTIFY pixelSizeChanged FINAL)
    Q_PROPERTY(QString familyName READ familyName NOTIFY familyNameChanged FINAL)

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

signals:
    void fileNameChanged();
    void isValidChanged();
    void pixelSizeChanged();
    void familyNameChanged();

private:
    QString m_fileName;
    QRawFont m_font;
};
