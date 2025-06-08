#include "FontLoaderObject.h"

namespace ui::textview::qml {

FontLoaderObject::FontLoaderObject(QObject *parent)
    : QObject(parent)
{
}

QString FontLoaderObject::fileName() const
{
    return m_loader.fileName();
}

void FontLoaderObject::setFileName(const QString &fileName)
{
    if (m_loader.fileName() == fileName)
        return;
    m_loader.loadFont(fileName);
    emit fileNameChanged();
    emit isValidChanged();
    emit familyNameChanged();
}

bool FontLoaderObject::isValid() const
{
    return m_loader.isValid();
}

qreal FontLoaderObject::pixelSize() const
{
    return m_loader.pixelSize();
}


int FontLoaderObject::glyphIndex(QChar character) const
{
    return m_loader.glyphIndex(character);
}

QRectF FontLoaderObject::boundingRect(int glyphIndex) const
{
    return m_loader.boundingRect(glyphIndex);
}

QString FontLoaderObject::familyName() const
{
    return m_loader.familyName();
}

const QRawFont &FontLoaderObject::font() const
{
    return m_loader.font();
}

FontLoader *FontLoaderObject::loader()
{
    return &m_loader;
}

} // namespace ui::textview::qml
