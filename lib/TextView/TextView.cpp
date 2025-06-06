#include "TextView.h"

TextView::TextView(QQuickItem *parent)
    : QQuickRhiItem(parent)
{
}

TextFramebuffer *TextView::framebuffer() const
{
    return m_framebuffer;
}

void TextView::setFramebuffer(TextFramebuffer *fb)
{
    if (m_framebuffer == fb)
        return;
    m_framebuffer = fb;
    emit framebufferChanged();
}

int TextView::width() const
{
    return m_width;
}

void TextView::setWidth(int w)
{
    if (m_width == w)
        return;
    m_width = w;
    emit widthChanged();
}

int TextView::height() const
{
    return m_height;
}

void TextView::setHeight(int h)
{
    if (m_height == h)
        return;
    m_height = h;
    emit heightChanged();
}

int TextView::posX() const
{
    return m_posX;
}

void TextView::setPosX(int x)
{
    if (m_posX == x)
        return;
    m_posX = x;
    emit posXChanged();
}

int TextView::posY() const
{
    return m_posY;
}

void TextView::setPosY(int y)
{
    if (m_posY == y)
        return;
    m_posY = y;
    emit posYChanged();
}

QSizeF TextView::characterSize() const
{
    return m_characterSize;
}

void TextView::setCharacterSize(const QSizeF &size)
{
    if (m_characterSize == size)
        return;
    m_characterSize = size;
    emit characterSizeChanged();
}

QQuickRhiItemRenderer *TextView::createRenderer()
{
    return nullptr;
}
