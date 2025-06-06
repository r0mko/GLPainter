#include "DFCache.h"
#include "FontLoader.h"
#include <QRhi>
#include <QDebug>
#include <QImage>

DFCache::DFCache(int cellsPerAtlas)
    : m_cellsPerAtlas(cellsPerAtlas)
{
}

DFCache::~DFCache()
{
    for (auto &a : m_atlases) {
        if (a.texture)
            a.texture->release();
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

    if (m_atlases.isEmpty() || m_atlases.last().next >= m_cellsPerAtlas) {
        Atlas a;
        QSize size(img.width() * m_cellsPerAtlas, img.height());
        a.texture = rhi->newTexture(QRhiTexture::R8, size, 1,
                                    QRhiTexture::UsedAsTransferDst | QRhiTexture::UsedAsSampled);
        if (!a.texture->create()) {
            qWarning() << "Failed to create atlas texture";
        }
        m_atlases.append(a);
    }

    Atlas &atlas = m_atlases.last();
    int indexInAtlas = atlas.next++;
    QPoint pos(img.width() * indexInAtlas, 0);
    QRhiTextureSubresourceUploadDescription subDesc(img);
    subDesc.setDestinationTopLeft(pos);
    QRhiTextureUploadEntry entry(0, 0, subDesc);
    QRhiTextureUploadDescription desc(entry);
    updates->uploadTexture(atlas.texture, desc);

    GlyphIndices gi{m_atlases.size() - 1, indexInAtlas};
    m_cache.insert(key, gi);
    return gi;
}

