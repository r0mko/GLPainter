#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include "TextFramebuffer.h"
#include "FontLoader.h"
#include "QmlTextAttributes.h"
#include <QFont>

namespace ui::textview::qml {

class TextFramebufferObject : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TextFramebufferObject(QObject *parent = nullptr);

    TextFramebuffer *buffer();
    const TextFramebuffer *buffer() const;
    ui::textview::FontLoader *loader();

    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)

    Q_INVOKABLE void putText(int row, int column, const QString &text, const TextAttributes &attr);
    Q_INVOKABLE void writeLn(const QString &text, const TextAttributes &attr);
    Q_INVOKABLE void clear(const QRect &rect);
    Q_INVOKABLE void fill(const QRect &rect, QChar ch, const TextAttributes &attr);

signals:
    void fontChanged();

private:
    TextFramebuffer m_buffer;
    QFont m_font;
    ui::textview::FontLoader m_loader;
};

} // namespace ui::textview::qml
