#include "DFGlyph.h"
#include "FontLoaderObject.h"
#include <QDebug>
#include <QPainterPath>

DFGlyph::DFGlyph(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
}

void DFGlyph::paint(QPainter *painter)
{
    qDebug() << "Painting DFGlyph size" << m_df.width() << m_df.height() << "glyphId" << m_df.glyph();
    painter->drawImage(m_topLeft.x(), implicitHeight() + m_topLeft.y(), m_df.toImage());
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    QPainterPath path;
    path.addRect(0, 0, implicitWidth(), implicitHeight());
    painter->setPen(pen);
    painter->strokePath(path, pen);
}

FontLoaderObject *DFGlyph::fontLoader() const
{
    return m_fontLoader;
}

void DFGlyph::setFontLoader(FontLoaderObject *fontLoader)
{
    if (m_fontLoader == fontLoader)
        return;
    m_fontLoader = fontLoader;
    emit fontLoaderChanged();
    if (!m_character.isNull()) {
        setDirty();
    }
}

QChar DFGlyph::character() const
{
    return m_character;
}

void DFGlyph::setCharacter(const QChar &character)
{
    if (m_character == character) {
        return;
    }
    m_character = character;
    qDebug() << "Setting character" << character;
    emit characterChanged();
    if (m_fontLoader) {
        setDirty();
    }
}

void DFGlyph::updateDF()
{
    const QRawFont &font = m_fontLoader->font();
    quint32 index = m_fontLoader->glyphIndex(m_character);
    QRectF rect = font.boundingRect(index);
    m_df.setGlyph(font, m_fontLoader->glyphIndex(m_character), true);
    setImplicitHeight(m_df.height());
    m_topLeft = { rect.left(), rect.top() - font.descent() };
    auto advance = font.advancesForGlyphIndexes({index}).at(0);
    setImplicitWidth(advance.x());
    setImplicitHeight(font.ascent() + font.descent());
    qDebug() << "Updating distance field glyph for character" << m_character << "rect" << rect << "rect width" << rect.width() << "df width" << m_df.width() << "height" << m_df.height() << "glyphId" << m_df.glyph() << "font" << m_fontLoader->familyName() << "ratio" << (m_df.width() - 10.0) / rect.width() << "advance" << advance;
}

void DFGlyph::setDirty()
{
    if (m_dirty) {
        return;
    }
    m_dirty = true;
    updateDF();
    update();
}
