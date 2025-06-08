#include "TextRenderer.h"
#include <QFile>
#include <QMatrix4x4>
#include <QHash>
#include <cstddef>

void TextRenderer::initialize(QRhi *rhi, QRhiRenderTarget *rt, QRhiCommandBuffer *cb)
{
    m_rhi = rhi;
    m_renderTarget = rt;

    static const float verts[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    m_vbuf = rhi->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(verts));
    m_vbuf->create();
    QRhiResourceUpdateBatch *u = rhi->nextResourceUpdateBatch();
    u->uploadStaticBuffer(m_vbuf, verts);
    cb->resourceUpdate(u);

    m_instBuf = rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::VertexBuffer, 0);
    m_instBuf->create();

    m_ubuf = rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer,
                             rhi->ubufAligned(sizeof(QMatrix4x4)));
    m_ubuf->create();

    m_sampler = rhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear,
                               QRhiSampler::None, QRhiSampler::ClampToEdge,
                               QRhiSampler::ClampToEdge);
    m_sampler->create();

    QFile vsFile(":/shaders/textview.vert.qsb");
    vsFile.open(QIODevice::ReadOnly);
    QShader vs = QShader::fromSerialized(vsFile.readAll());
    QFile fsFile(":/shaders/textview.frag.qsb");
    fsFile.open(QIODevice::ReadOnly);
    QShader fs = QShader::fromSerialized(fsFile.readAll());

    m_srb = rhi->newShaderResourceBindings();
    m_srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, m_ubuf),
        QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, nullptr, m_sampler)
    });
    m_srb->create();

    QRhiGraphicsPipeline *ps = rhi->newGraphicsPipeline();
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
    ps->setRenderPassDescriptor(rt->renderPassDescriptor());
    ps->setTopology(QRhiGraphicsPipeline::TriangleStrip);
    m_pipe = ps;
    m_pipe->create();
}

void TextRenderer::prepare(const QVector<RenderCell> &cells,
                           FontLoader *fontLoader,
                           int viewPosX,
                           int viewPosY,
                           int viewWidth,
                           int viewHeight)
{
    m_cells = cells;
    m_fontLoader = fontLoader;
    m_viewPosX = viewPosX;
    m_viewPosY = viewPosY;
    m_viewWidth = viewWidth;
    m_viewHeight = viewHeight;
    if (m_fontLoader) {
        const QRawFont &font = m_fontLoader->font();
        m_charWidth = font.averageCharWidth();
        m_charHeight = font.ascent() + font.descent();
    } else {
        m_charWidth = 0.f;
        m_charHeight = 0.f;
    }
}

void TextRenderer::render(QRhiCommandBuffer *cb)
{
    if (!m_fontLoader || m_cells.isEmpty())
        return;

    QRhi *r = m_rhi;
    QRhiResourceUpdateBatch *u = r->nextResourceUpdateBatch();

    const QRawFont &font = m_fontLoader->font();
    float charW = font.averageCharWidth();
    float charH = font.ascent() + font.descent();

    QMatrix4x4 m;
    m.ortho(0.0f, float(m_viewWidth) * charW, float(m_viewHeight) * charH, 0.0f, -1.0f, 1.0f);
    u->updateDynamicBuffer(m_ubuf, 0, sizeof(QMatrix4x4), m.constData());

    QHash<FontLoader *, QVector<QVector<InstanceData>>> groups;
    for (const RenderCell &cell : m_cells) {
        const TextAttributes::Data &attr = cell.attributes.data();
        FontLoader *fl = attr.font ? attr.font : m_fontLoader;
        auto gi = fl->glyph(cell.character, r, u);
        if (gi.atlas < 0)
            continue;
        QSize cellSz = fl->cellSize();
        QSize atlasSz(cellSz.width() * fl->cellsPerRow(), cellSz.height() * fl->cellsPerRow());
        int row = gi.index / fl->cellsPerRow();
        int col = gi.index % fl->cellsPerRow();
        InstanceData id;
        id.pos = QVector2D((cell.position.x() - m_viewPosX) * cellSz.width(),
                            (cell.position.y() - m_viewPosY) * cellSz.height());
        id.size = QVector2D(cellSz.width(), cellSz.height());
        id.uvOrigin = QVector2D(float(col * cellSz.width()) / atlasSz.width(),
                                float(row * cellSz.height()) / atlasSz.height());
        id.uvSize = QVector2D(float(cellSz.width()) / atlasSz.width(),
                              float(cellSz.height()) / atlasSz.height());
        id.color = QVector4D(attr.color.redF(), attr.color.greenF(), attr.color.blueF(), attr.color.alphaF());
        QVector<QVector<InstanceData>> &vec = groups[fl];
        if (vec.size() <= gi.atlas)
            vec.resize(gi.atlas + 1);
        vec[gi.atlas].append(id);
    }

    cb->beginPass(m_renderTarget, Qt::transparent, {1.0f, 0}, u);
    cb->setGraphicsPipeline(m_pipe);
    cb->setViewport({0, 0, float(m_renderTarget->pixelSize().width()), float(m_renderTarget->pixelSize().height())});
    QRhiCommandBuffer::VertexInput vbufs[] = {
        { m_vbuf, 0 },
        { m_instBuf, 0 }
    };
    cb->setVertexInput(0, 2, vbufs);

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        FontLoader *fl = it.key();
        const auto &vec = it.value();
        for (int i = 0; i < vec.size(); ++i) {
            const auto &insts = vec[i];
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
                QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, fl->atlasTexture(i), m_sampler)
            });
            m_srb->create();
            cb->setShaderResources(m_srb);
            cb->draw(4, insts.size());
        }
    }

    cb->endPass();
}

QSizeF TextRenderer::pixelSize() const
{
    return QSizeF(m_viewWidth * m_charWidth, m_viewHeight * m_charHeight);
}
