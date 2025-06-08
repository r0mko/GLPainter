#include "QuickTextViewRenderer.h"
#include "TextView.h"
#include "FontLoaderObject.h"
#include "TextFramebufferObject.h"

namespace ui::textview::qml {

void QuickTextViewRenderer::initialize(QRhiCommandBuffer *cb)
{
    m_renderer.initialize(rhi(), renderTarget(), cb);
}

void QuickTextViewRenderer::synchronize(QQuickRhiItem *item)
{
    m_view = static_cast<TextView *>(item);
    if (m_view && m_view->framebuffer()) {
        QRect region(m_view->posX(), m_view->posY(), m_view->renderWidth(), m_view->renderHeight());
        m_cells = m_view->framebuffer()->buffer()->collect(region);
    } else {
        m_cells.clear();
    }
    ui::textview::FontLoader *fl = nullptr;
    if (m_view) {
        if (m_view->fontLoader())
            fl = m_view->fontLoader()->loader();
        else if (m_view->framebuffer())
            fl = m_view->framebuffer()->loader();
    }
    m_renderer.prepare(m_cells,
                       fl,
                       m_view ? m_view->posX() : 0, m_view ? m_view->posY() : 0,
                       m_view ? m_view->renderWidth() : 0, m_view ? m_view->renderHeight() : 0);
}

void QuickTextViewRenderer::render(QRhiCommandBuffer *cb)
{
    if (!m_view || m_cells.isEmpty())
        return;
    if (!m_view->fontLoader() && !(m_view->framebuffer() && m_view->framebuffer()->loader()))
        return;
    m_renderer.render(cb);
}

} // namespace ui::textview::qml
