#pragma once

#include <QQuickRhiItem>
#include <QVector>
#include <QVector2D>
#include <QVector4D>
#include "DFCache.h"
#include "TextFramebuffer.h"

class TextView;

class TextViewRenderer : public QQuickRhiItemRenderer
{
public:
    TextViewRenderer() = default;

protected:
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    struct InstanceData {
        QVector2D pos;
        QVector2D size;
        QVector2D uvOrigin;
        QVector2D uvSize;
        QVector4D color;
    };

    TextView *m_view = nullptr;
    QVector<RenderCell> m_cells;

    QRhiBuffer *m_vbuf = nullptr;
    QRhiBuffer *m_instBuf = nullptr;
    QRhiBuffer *m_ubuf = nullptr;
    QRhiGraphicsPipeline *m_pipe = nullptr;
    QRhiShaderResourceBindings *m_srb = nullptr;
    QRhiSampler *m_sampler = nullptr;

    DFCache m_cache;
};

