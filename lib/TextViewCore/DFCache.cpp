#include "DFCache.h"
#include "FontLoader.h"
#include <QtGui/6.9.1/QtGui/rhi/qrhi.h>
#include <QDebug>
#include <QImage>
#include <cmath>

DFCache::DFCache(int cellsPerAtlas)
    : m_cellsPerAtlas(cellsPerAtlas)
{
}

DFCache::~DFCache()
{
    for (auto &a : m_atlases) {
        delete a.texture;
    }
}

DFCache::GlyphIndices DFCache::glyph(FontLoader *font,
                                     QChar ch,
                                     QRhi *rhi,
                                     QRhiResourceUpdateBatch *updates)
{
    Key key{font, ch.unicode()};
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
    return addGlyph(key, font, ch, rhi, updates);
}

DFCache::GlyphIndices DFCache::addGlyph(const Key &key,
                                        FontLoader *font,
                                        QChar ch,
                                        QRhi *rhi,
                                        QRhiResourceUpdateBatch *updates)
{
    Q_ASSERT(rhi);
    Q_ASSERT(updates);

    QDistanceField df;
    const QRawFont &f = font->font();
    int idx = font->glyphIndex(ch);
    df.setGlyph(f, idx, true);
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
        if (!a.texture->create()) {
            qWarning() << "Failed to create atlas texture";
        }
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
    m_cache.insert(key, gi);
    return gi;
}

QSize DFCache::cellSize() const
{
    return m_cellSize;
}

int DFCache::cellsPerRow() const
{
    return m_cellsPerRow;
}

int DFCache::atlasCount() const
{
    return m_atlases.size();
}

QRhiTexture *DFCache::atlasTexture(int index) const
{
    if (index < 0 || index >= m_atlases.size())
        return nullptr;
    return m_atlases[index].texture;
}

