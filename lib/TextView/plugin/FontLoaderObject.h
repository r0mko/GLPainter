#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include "FontLoader.h"

namespace ui::textview::qml {

class FontLoaderObject : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)
    Q_PROPERTY(qreal pixelSize READ pixelSize FINAL)
    Q_PROPERTY(QString familyName READ familyName NOTIFY familyNameChanged FINAL)
public:
    explicit FontLoaderObject(QObject *parent = nullptr);

    QString fileName() const;
    void setFileName(const QString &fileName);
    bool isValid() const;

    qreal pixelSize() const;

    int glyphIndex(QChar character) const;
    QRectF boundingRect(int glyphIndex) const;

    QString familyName() const;

    const QRawFont &font() const;
    FontLoader *loader();

signals:
    void fileNameChanged();
    void isValidChanged();
    void familyNameChanged();

private:
    ui::textview::FontLoader m_loader;
};

} // namespace ui::textview::qml
