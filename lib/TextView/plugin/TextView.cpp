#include "TextView.h"
#include "QuickTextViewRenderer.h"
#include <QRawFont>
#include <QDebug>

namespace ui::textview::qml {
TextView::TextView(QQuickItem *parent)
    : QQuickRhiItem(parent)
{
}

TextFramebufferObject *TextView::framebuffer() const
{
    return m_framebuffer;
}

FontLoaderObject *TextView::fontLoader() const
{
    return m_fontLoader;
}

void TextView::setFontLoader(FontLoaderObject *loader)
{
    if (m_fontLoader == loader)
        return;
    m_fontLoader = loader;
    if (m_fontLoader)
        m_fontSize = m_fontLoader->loader()->pixelSize();
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
    updateImplicitSize();
    emit fontSizeChanged();
    update();
}
void TextView::setFramebuffer(TextFramebufferObject *fb)
{
    if (m_framebuffer == fb)
        return;
    m_framebuffer = fb;
    emit framebufferChanged();
    update();
}

int TextView::renderWidth() const
{
    return m_renderWidth;
}

void TextView::setRenderWidth(int w)
{
    if (m_renderWidth == w)
        return;
    m_renderWidth = w;
    updateImplicitSize();
    emit renderWidthChanged();
    update();
}

int TextView::renderHeight() const
{
    return m_renderHeight;
}

void TextView::setRenderHeight(int h)
{
    if (m_renderHeight == h)
        return;
    m_renderHeight = h;
    updateImplicitSize();
    emit renderHeightChanged();
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
    setImplicitWidth(m_renderWidth * charWidth);
    setImplicitHeight(m_renderHeight * charHeight);
}

} // namespace ui::textview::qml
