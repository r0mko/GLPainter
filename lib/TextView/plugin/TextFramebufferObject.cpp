#include "TextFramebufferObject.h"
#include "QmlTextAttributes.h"

namespace ui::textview::qml {

TextFramebufferObject::TextFramebufferObject(QObject *parent)
    : QObject(parent)
{
    m_loader.setFont(m_font);
}

TextFramebuffer *TextFramebufferObject::buffer()
{
    return &m_buffer;
}

const TextFramebuffer *TextFramebufferObject::buffer() const
{
    return &m_buffer;
}

ui::textview::FontLoader *TextFramebufferObject::loader()
{
    return &m_loader;
}

QFont TextFramebufferObject::font() const
{
    return m_font;
}

void TextFramebufferObject::setFont(const QFont &font)
{
    if (m_font == font)
        return;
    m_font = font;
    m_loader.setFont(m_font);
    emit fontChanged();
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

} // namespace ui::textview::qml
