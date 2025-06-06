#pragma once

#include <QHash>
#include <QReadWriteLock>
#include <QtGui/6.9.1/QtGui/rhi/qrhi.h>
#include <QVector>
#include <private/qdistancefield_p.h>

class FontLoader;
class QRhi;
class QRhiResourceUpdateBatch;

/*!\brief Glyph cache for distance field textures.
 */
class DFCache
{
public:
    struct GlyphIndices {
        int atlas = -1;  //!< index of the atlas texture
        int index = -1;  //!< index of the glyph within the atlas
    };

    explicit DFCache(int cellsPerAtlas = 256);
    ~DFCache();

    GlyphIndices glyph(FontLoader *font,
                       QChar ch,
                       QRhi *rhi,
                       QRhiResourceUpdateBatch *updates);

private:
    struct Key {
        FontLoader *font;
        char16_t ch;
    };
    friend bool operator==(const Key &a, const Key &b) noexcept
    {
        return a.font == b.font && a.ch == b.ch;
    }

    friend size_t qHash(const Key &k, size_t seed = 0) noexcept
    {
        return qHash(quintptr(k.font), seed) ^ qHash(k.ch, seed);
    }

    struct Atlas {
        QRhiTexture *texture = nullptr;
        int next = 0;
    };

    GlyphIndices addGlyph(const Key &key,
                          FontLoader *font,
                          QChar ch,
                          QRhi *rhi,
                          QRhiResourceUpdateBatch *updates);

    const int m_cellsPerAtlas;
    QReadWriteLock m_lock;
    QHash<Key, GlyphIndices> m_cache;
    QVector<Atlas> m_atlases;
};

