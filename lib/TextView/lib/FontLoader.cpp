#include "FontLoader.h"
#include <QDebug>
#include <cmath>

FontLoader::FontLoader(qreal pixelSize, int cellsPerAtlas)
    : m_pixelSize(pixelSize)
    , m_cellsPerAtlas(cellsPerAtlas)
{
}

FontLoader::~FontLoader()
{
    for (auto &a : m_atlases)
        delete a.texture;
}

QString FontLoader::fileName() const
{
    return m_fileName;
}

bool FontLoader::loadFont(const QString &fileName)
{
    if (m_fileName == fileName && m_font.isValid())
        return true;
    m_fileName = fileName;
    qDebug() << "Loading font from" << fileName;
    m_font.loadFromFile(m_fileName, m_pixelSize, QFont::PreferDefaultHinting);

    if (m_font.isValid()) {
        qDebug() << "Loaded font" << m_font.familyName();
        return true;
    } else {
        qWarning() << "Failed to load font" << fileName;
        return false;
    }
}

bool FontLoader::isValid() const
{
    return m_font.isValid();
}

qreal FontLoader::pixelSize() const
{
    return m_pixelSize;
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

FontLoader::GlyphIndices FontLoader::glyph(QChar ch, QRhi *rhi, QRhiResourceUpdateBatch *updates)
{
    char16_t key = ch.unicode();
    {
        QReadLocker rl(&m_lock);
        auto it = m_cache.constFind(key);
        if (it != m_cache.cend())
            return it.value();
    }
    QWriteLocker wl(&m_lock);
    auto it = m_cache.constFind(key);
    if (it != m_cache.cend())
        return it.value();
    return addGlyph(key, rhi, updates);
}

FontLoader::GlyphIndices FontLoader::addGlyph(char16_t ch, QRhi *rhi, QRhiResourceUpdateBatch *updates)
{
    Q_ASSERT(rhi);
    Q_ASSERT(updates);

    QDistanceField df;
    int idx = glyphIndex(QChar(ch));
    df.setGlyph(m_font, idx, true);
    QImage img = df.toImage(QImage::Format_Grayscale8);

    if (m_cellSize.isEmpty()) {
        m_cellSize = img.size();
        m_cellsPerRow = qMax(1, int(std::sqrt(double(m_cellsPerAtlas))));
    }

    if (m_atlases.isEmpty() || m_atlases.last().next >= m_cellsPerAtlas) {
        Atlas a;
        QSize texSize(m_cellSize.width() * m_cellsPerRow,
                      m_cellSize.height() * m_cellsPerRow);
        a.texture = rhi->newTexture(QRhiTexture::R8, texSize, 1);
        if (!a.texture->create())
            qWarning() << "Failed to create atlas texture";
        m_atlases.append(a);
    }

    Atlas &atlas = m_atlases.last();
    int indexInAtlas = atlas.next++;
    int row = indexInAtlas / m_cellsPerRow;
    int col = indexInAtlas % m_cellsPerRow;
    QPoint pos(col * m_cellSize.width(), row * m_cellSize.height());
    QRhiTextureSubresourceUploadDescription subDesc(img);
    subDesc.setDestinationTopLeft(pos);
    QRhiTextureUploadEntry entry(0, 0, subDesc);
    QRhiTextureUploadDescription desc(entry);
    updates->uploadTexture(atlas.texture, desc);

    GlyphIndices gi{static_cast<int>(m_atlases.size()) - 1, indexInAtlas};
    m_cache.insert(ch, gi);
    return gi;
}

QSize FontLoader::cellSize() const
{
    return m_cellSize;
}

int FontLoader::cellsPerRow() const
{
    return m_cellsPerRow;
}

int FontLoader::atlasCount() const
{
    return m_atlases.size();
}

QRhiTexture *FontLoader::atlasTexture(int index) const
{
    if (index < 0 || index >= m_atlases.size())
        return nullptr;
    return m_atlases[index].texture;
}
