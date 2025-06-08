#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include "TextFramebuffer.h"

class TextFramebufferObject : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TextFramebufferObject(QObject *parent = nullptr);

    TextFramebuffer *buffer();
    const TextFramebuffer *buffer() const;

    Q_INVOKABLE void putText(int row, int column, const QString &text, const TextAttributes &attr);
    Q_INVOKABLE void writeLn(const QString &text, const TextAttributes &attr);
    Q_INVOKABLE void clear(const QRect &rect);
    Q_INVOKABLE void fill(const QRect &rect, QChar ch, const TextAttributes &attr);

private:
    TextFramebuffer m_buffer;
};
