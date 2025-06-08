#include "TextFramebufferObject.h"

TextFramebufferObject::TextFramebufferObject(QObject *parent)
    : QObject(parent)
{
}

TextFramebuffer *TextFramebufferObject::buffer()
{
    return &m_buffer;
}

const TextFramebuffer *TextFramebufferObject::buffer() const
{
    return &m_buffer;
}

void TextFramebufferObject::putText(int row, int column, const QString &text, const TextAttributes &attr)
{
    m_buffer.putText(row, column, text, attr);
}

void TextFramebufferObject::writeLn(const QString &text, const TextAttributes &attr)
{
    m_buffer.writeLn(text, attr);
}

void TextFramebufferObject::clear(const QRect &rect)
{
    m_buffer.clear(rect);
}

void TextFramebufferObject::fill(const QRect &rect, QChar ch, const TextAttributes &attr)
{
    m_buffer.fill(rect, ch, attr);
}
