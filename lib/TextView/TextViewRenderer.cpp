#include "TextViewRenderer.h"
#include "TextView.h"
#include "FontLoader.h"
#include <QFile>
#include <QMatrix4x4>
#include <cstddef>

void TextViewRenderer::initialize(QRhiCommandBuffer *cb)
{
    Q_UNUSED(cb);
    QRhi *r = rhi();

    static const float verts[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    m_vbuf = r->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer,
                           sizeof(verts));
    m_vbuf->create();
    QRhiResourceUpdateBatch *u = r->nextResourceUpdateBatch();
    u->uploadStaticBuffer(m_vbuf, verts);
    cb->resourceUpdate(u);

    m_instBuf = r->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::VertexBuffer, 0);
    m_instBuf->create();

    m_ubuf = r->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer,
                          r->ubufAligned(sizeof(QMatrix4x4)));
    m_ubuf->create();

    m_sampler = r->newSampler(QRhiSampler::Linear, QRhiSampler::Linear,
                              QRhiSampler::None, QRhiSampler::ClampToEdge,
                              QRhiSampler::ClampToEdge);
    m_sampler->create();

    QFile vsFile(":/shaders/textview.vert.qsb");
    vsFile.open(QIODevice::ReadOnly);
    QShader vs = QShader::fromSerialized(vsFile.readAll());
    QFile fsFile(":/shaders/textview.frag.qsb");
    fsFile.open(QIODevice::ReadOnly);
    QShader fs = QShader::fromSerialized(fsFile.readAll());

    m_srb = r->newShaderResourceBindings();
    m_srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, m_ubuf),
        QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, nullptr, m_sampler)
    });
    m_srb->create();

    QRhiGraphicsPipeline *ps = r->newGraphicsPipeline();
    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings({
        {4 * sizeof(float)},
        {sizeof(InstanceData), QRhiVertexInputBinding::PerInstance}
    });
    inputLayout.setAttributes({
        {0, 0, QRhiVertexInputAttribute::Float2, 0},
        {0, 1, QRhiVertexInputAttribute::Float2, 2 * sizeof(float)},
        {1, 2, QRhiVertexInputAttribute::Float2, offsetof(InstanceData, pos)},
        {1, 3, QRhiVertexInputAttribute::Float2, offsetof(InstanceData, size)},
        {1, 4, QRhiVertexInputAttribute::Float2, offsetof(InstanceData, uvOrigin)},
        {1, 5, QRhiVertexInputAttribute::Float2, offsetof(InstanceData, uvSize)},
        {1, 6, QRhiVertexInputAttribute::Float4, offsetof(InstanceData, color)}
    });
    ps->setShaderStages({
        {QRhiShaderStage::Vertex, vs},
        {QRhiShaderStage::Fragment, fs}
    });
    ps->setVertexInputLayout(inputLayout);
    ps->setShaderResourceBindings(m_srb);
    ps->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    ps->setTopology(QRhiGraphicsPipeline::TriangleStrip);
    m_pipe = ps;
    m_pipe->create();
}

void TextViewRenderer::synchronize(QQuickRhiItem *item)
{
    m_view = static_cast<TextView *>(item);
    if (m_view && m_view->framebuffer()) {
        QRect region(m_view->posX(), m_view->posY(), m_view->width(), m_view->height());
        m_cells = m_view->framebuffer()->collect(region);
    } else {
        m_cells.clear();
    }
}

void TextViewRenderer::render(QRhiCommandBuffer *cb)
{
    if (!m_view || !m_view->fontLoader() || m_cells.isEmpty())
        return;

    QRhi *r = rhi();
    QRhiResourceUpdateBatch *u = r->nextResourceUpdateBatch();

    const QRawFont &font = m_view->fontLoader()->font();
    float charW = font.averageCharWidth();
    float charH = font.ascent() + font.descent();

    QMatrix4x4 m;
    m.ortho(0.0f, float(m_view->width()) * charW, float(m_view->height()) * charH, 0.0f, -1.0f, 1.0f);
    u->updateDynamicBuffer(m_ubuf, 0, sizeof(QMatrix4x4), m.constData());

    QVector<QVector<InstanceData>> groups(m_cache.atlasCount());
    QSize cellSz = m_cache.cellSize();
    QSize atlasSz(cellSz.width() * m_cache.cellsPerRow(), cellSz.height() * m_cache.cellsPerRow());
    for (const RenderCell &cell : m_cells) {
        const TextAttributes::Data &attr = cell.attributes.data();
        FontLoader *fl = attr.font ? attr.font : m_view->fontLoader();
        auto gi = m_cache.glyph(fl, cell.character, r, u);
        if (gi.atlas < 0)
            continue;
        int row = gi.index / m_cache.cellsPerRow();
        int col = gi.index % m_cache.cellsPerRow();
        InstanceData id;
        id.pos = QVector2D((cell.position.x() - m_view->posX()) * cellSz.width(),
                            (cell.position.y() - m_view->posY()) * cellSz.height());
        id.size = QVector2D(cellSz.width(), cellSz.height());
        id.uvOrigin = QVector2D(float(col * cellSz.width()) / atlasSz.width(),
                                float(row * cellSz.height()) / atlasSz.height());
        id.uvSize = QVector2D(float(cellSz.width()) / atlasSz.width(),
                              float(cellSz.height()) / atlasSz.height());
        id.color = QVector4D(attr.color.redF(), attr.color.greenF(), attr.color.blueF(), attr.color.alphaF());
        if (gi.atlas >= groups.size())
            groups.resize(gi.atlas + 1);
        groups[gi.atlas].append(id);
    }

    cb->beginPass(renderTarget(), Qt::transparent, {1.0f, 0}, u);
    cb->setGraphicsPipeline(m_pipe);
    cb->setViewport({0, 0, float(renderTarget()->pixelSize().width()), float(renderTarget()->pixelSize().height())});
    QRhiCommandBuffer::VertexInput vbufs[] = {
        { m_vbuf, 0 },
        { m_instBuf, 0 }
    };
    cb->setVertexInput(0, 2, vbufs);

    for (int i = 0; i < groups.size(); ++i) {
        const auto &insts = groups[i];
        if (insts.isEmpty())
            continue;
        qsizetype size = insts.size() * sizeof(InstanceData);
        if (m_instBuf->size() < size) {
            m_instBuf->setSize(size);
            m_instBuf->create();
        }
        u = r->nextResourceUpdateBatch();
        u->updateDynamicBuffer(m_instBuf, 0, size, insts.constData());
        cb->resourceUpdate(u);

        delete m_srb;
        m_srb = r->newShaderResourceBindings();
        m_srb->setBindings({
            QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, m_ubuf),
            QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, m_cache.atlasTexture(i), m_sampler)
        });
        m_srb->create();
        cb->setShaderResources(m_srb);
        cb->draw(4, insts.size());
    }

    cb->endPass();
}

