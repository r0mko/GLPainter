#pragma once

#include <QQuickRhiItem>
#include <QtQml/qqml.h>
#include <QSizeF>
#include "TextFramebufferObject.h"
#include "FontLoaderObject.h"

namespace ui::textview::qml {

class TextView : public QQuickRhiItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TextFramebufferObject* framebuffer READ framebuffer WRITE setFramebuffer NOTIFY framebufferChanged)
    Q_PROPERTY(int renderWidth READ renderWidth WRITE setRenderWidth NOTIFY renderWidthChanged)
    Q_PROPERTY(int renderHeight READ renderHeight WRITE setRenderHeight NOTIFY renderHeightChanged)
    Q_PROPERTY(int posX READ posX WRITE setPosX NOTIFY posXChanged)
    Q_PROPERTY(int posY READ posY WRITE setPosY NOTIFY posYChanged)
    Q_PROPERTY(qreal fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(FontLoaderObject* fontLoader READ fontLoader WRITE setFontLoader NOTIFY fontLoaderChanged)
public:
    explicit TextView(QQuickItem *parent = nullptr);

    TextFramebufferObject *framebuffer() const;
    void setFramebuffer(TextFramebufferObject *fb);

    FontLoaderObject *fontLoader() const;
    void setFontLoader(FontLoaderObject *loader);

    qreal fontSize() const;
    void setFontSize(qreal size);

    int renderWidth() const;
    void setRenderWidth(int w);

    int renderHeight() const;
    void setRenderHeight(int h);

    int posX() const;
    void setPosX(int x);

    int posY() const;
    void setPosY(int y);


signals:
    void framebufferChanged();
    void renderWidthChanged();
    void renderHeightChanged();
    void posXChanged();
    void posYChanged();
    void fontLoaderChanged();
    void fontSizeChanged();

protected:
    QQuickRhiItemRenderer *createRenderer() override;

private:
    TextFramebufferObject *m_framebuffer = nullptr;
    int m_renderWidth = 0;
    int m_renderHeight = 0;
    int m_posX = 0;
    int m_posY = 0;
    FontLoaderObject *m_fontLoader = nullptr;
    qreal m_fontSize = 12.0;

    void updateImplicitSize();
};

} // namespace ui::textview::qml
