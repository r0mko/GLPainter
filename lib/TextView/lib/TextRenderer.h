#pragma once

#include <QVector>
#include <QVector2D>
#include <QVector4D>
#include <QtGui/6.9.1/QtGui/rhi/qrhi.h>
#include <QSizeF>
#include "TextFramebuffer.h"
#include "FontLoader.h"

namespace ui::textview {

class TextRenderer
{
public:
    TextRenderer() = default;

    void initialize(QRhi *rhi, QRhiRenderTarget *rt, QRhiCommandBuffer *cb);
    void prepare(const QVector<RenderCell> &cells,
                 FontLoader *fontLoader,
                 int viewPosX,
                 int viewPosY,
                 int viewWidth,
                 int viewHeight);
    void render(QRhiCommandBuffer *cb);

    QSizeF pixelSize() const;

private:
    struct InstanceData {
        QVector2D pos;
        QVector2D size;
        QVector2D uvOrigin;
        QVector2D uvSize;
        QVector4D color;
    };

    QRhi *m_rhi = nullptr;
    QRhiRenderTarget *m_renderTarget = nullptr;
    QVector<RenderCell> m_cells;
    FontLoader *m_fontLoader = nullptr;
    int m_viewPosX = 0;
    int m_viewPosY = 0;
    int m_viewWidth = 0;
    int m_viewHeight = 0;
    float m_charWidth = 0.f;
    float m_charHeight = 0.f;

    QRhiBuffer *m_vbuf = nullptr;
    QRhiBuffer *m_instBuf = nullptr;
    QRhiBuffer *m_ubuf = nullptr;
    QRhiGraphicsPipeline *m_pipe = nullptr;
    QRhiShaderResourceBindings *m_srb = nullptr;
    QRhiSampler *m_sampler = nullptr;
};

} // namespace ui::textview
