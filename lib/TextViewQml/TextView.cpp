#include "TextView.h"
#include "QuickTextViewRenderer.h"
#include <QRawFont>
#include <QDebug>

TextView::TextView(QQuickItem *parent)
    : QQuickRhiItem(parent)
{
}

TextFramebuffer *TextView::framebuffer() const
{
    return m_framebuffer;
}

FontLoader *TextView::fontLoader() const
{
    return m_fontLoader;
}

void TextView::setFontLoader(FontLoader *loader)
{
    if (m_fontLoader == loader)
        return;
    m_fontLoader = loader;
    if (m_fontLoader)
        m_fontSize = m_fontLoader->pixelSize();
    updateImplicitSize();
    emit fontLoaderChanged();
    emit fontSizeChanged();
    update();
}

qreal TextView::fontSize() const
{
    return m_fontSize;
}

void TextView::setFontSize(qreal size)
{
    if (qFuzzyCompare(m_fontSize, size))
        return;
    m_fontSize = size;
    if (m_fontLoader)
        m_fontLoader->setPixelSize(size);
    updateImplicitSize();
    emit fontSizeChanged();
    update();
}
void TextView::setFramebuffer(TextFramebuffer *fb)
{
    if (m_framebuffer == fb)
        return;
    m_framebuffer = fb;
    emit framebufferChanged();
    update();
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
    updateImplicitSize();
    emit widthChanged();
    update();
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
    updateImplicitSize();
    emit heightChanged();
    update();
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
    update();
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
    update();
}

QQuickRhiItemRenderer *TextView::createRenderer()
{
    return new QuickTextViewRenderer;
}

void TextView::updateImplicitSize()
{
    if (!m_fontLoader)
        return;
    const QRawFont &f = m_fontLoader->font();
    const qreal charWidth = f.averageCharWidth();
    const qreal charHeight = f.ascent() + f.descent();
    setImplicitWidth(m_width * charWidth);
    setImplicitHeight(m_height * charHeight);
}
