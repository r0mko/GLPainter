#pragma once

#include <QQuickRhiItem>
#include <QVector>
#include "TextRenderer.h"
#include "TextFramebuffer.h"

class TextView;

class QuickTextViewRenderer : public QQuickRhiItemRenderer
{
public:
    QuickTextViewRenderer() = default;

protected:
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    TextView *m_view = nullptr;
    QVector<RenderCell> m_cells;
    TextRenderer m_renderer;
};
