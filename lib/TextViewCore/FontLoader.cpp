#include "FontLoader.h"
#include <QDebug>

FontLoader::FontLoader() {}

QString FontLoader::fileName() const
{
    return m_fileName;
}

void FontLoader::setFileName(const QString &fileName)
{
    if (m_fileName == fileName)
        return;
    bool oldIsValid = m_font.isValid();
    QString oldFamilyName = m_font.familyName();
    m_fileName = fileName;
    qDebug() << "Loading font from" << fileName;
    m_font.loadFromFile(m_fileName, 120.0, QFont::PreferDefaultHinting);

    if (m_font.isValid()) {
        qDebug() << "Loaded font" << m_font.familyName();
    } else {
        qWarning() << "Failed to load font" << fileName;
    }

    if (m_font.isValid() != oldIsValid) {
        emit isValidChanged();
    }

    if (m_font.familyName() != oldFamilyName)
        emit familyNameChanged();
}

bool FontLoader::isValid() const
{
    return m_font.isValid();
}

qreal FontLoader::pixelSize() const
{
    return m_font.pixelSize();
}

void FontLoader::setPixelSize(qreal pixelSize)
{
    if (qFuzzyCompare(m_font.pixelSize(), pixelSize))
        return;
    m_font.setPixelSize(pixelSize);
    emit pixelSizeChanged();
}

int FontLoader::glyphIndex(QChar character) const
{
    return m_font.glyphIndexesForString({character}).at(0);
}

QString FontLoader::familyName() const
{
    return m_font.familyName();
}

const QRawFont &FontLoader::font() const
{
    return m_font;
}

QRawFont &FontLoader::font()
{
    return m_font;
}
