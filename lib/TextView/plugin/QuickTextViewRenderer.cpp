#include "QuickTextViewRenderer.h"
#include "TextView.h"
#include "FontLoaderObject.h"
#include "TextFramebufferObject.h"

void QuickTextViewRenderer::initialize(QRhiCommandBuffer *cb)
{
    m_renderer.initialize(rhi(), renderTarget(), cb);
}

void QuickTextViewRenderer::synchronize(QQuickRhiItem *item)
{
    m_view = static_cast<TextView *>(item);
    if (m_view && m_view->framebuffer()) {
        QRect region(m_view->posX(), m_view->posY(), m_view->width(), m_view->height());
        m_cells = m_view->framebuffer()->buffer()->collect(region);
    } else {
        m_cells.clear();
    }
    m_renderer.prepare(m_cells,
                       m_view && m_view->fontLoader() ? m_view->fontLoader()->loader() : nullptr,
                       m_view ? m_view->posX() : 0, m_view ? m_view->posY() : 0,
                       m_view ? m_view->width() : 0, m_view ? m_view->height() : 0);
}

void QuickTextViewRenderer::render(QRhiCommandBuffer *cb)
{
    if (!m_view || !m_view->fontLoader() || m_cells.isEmpty())
        return;
    m_renderer.render(cb);
}
