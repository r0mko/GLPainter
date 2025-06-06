#pragma once

#include <QHash>
#include <QReadWriteLock>
#include <QRhiTexture>
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
    struct KeyHash {
        size_t operator()(const Key &k) const noexcept {
            return qHash(quintptr(k.font)) ^ qHash(static_cast<quint32>(k.ch));
        }
    };
    struct KeyEq {
        bool operator()(const Key &a, const Key &b) const noexcept {
            return a.font == b.font && a.ch == b.ch;
        }
    };

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
    QHash<Key, GlyphIndices, KeyHash, KeyEq> m_cache;
    QVector<Atlas> m_atlases;
};

