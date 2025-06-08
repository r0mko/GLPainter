#pragma once

#include <QRawFont>
#include <QFont>
#include <QHash>
#include <QReadWriteLock>
#include <QtGui/6.9.1/QtGui/rhi/qrhi.h>
#include <private/qdistancefield_p.h>

namespace ui::textview {

class FontLoader
{
public:
    struct GlyphIndices {
        int atlas = -1;
        int index = -1;
    };

    explicit FontLoader(qreal pixelSize = 12.0, int cellsPerAtlas = 256);
    ~FontLoader();

    QString fileName() const;
    bool loadFont(const QString &fileName);
    bool setFont(const QFont &font);
    bool isValid() const;

    qreal pixelSize() const;

    int glyphIndex(QChar character) const;
    QRectF boundingRect(int glyphIndex) const;
    QString familyName() const;

    const QRawFont &font() const;
    QRawFont &font();

    GlyphIndices glyph(QChar ch, QRhi *rhi, QRhiResourceUpdateBatch *updates);
    QSize cellSize() const;
    int cellsPerRow() const;
    int atlasCount() const;
    QRhiTexture *atlasTexture(int index) const;

private:
    GlyphIndices addGlyph(char16_t ch, QRhi *rhi, QRhiResourceUpdateBatch *updates);

    QString m_fileName;
    qreal m_pixelSize = 12.0;
    QRawFont m_font;

    const int m_cellsPerAtlas;
    QReadWriteLock m_lock;
    QHash<char16_t, GlyphIndices> m_cache;
    struct Atlas {
        QRhiTexture *texture = nullptr;
        int next = 0;
    };
    QVector<Atlas> m_atlases;
    QSize m_cellSize;
    int m_cellsPerRow = 1;
};

} // namespace ui::textview
