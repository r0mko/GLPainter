#pragma once

#include <QQuickRhiItem>
#include <QtQml/qqml.h>
#include <QSizeF>
#include "TextFramebuffer.h"
#include "FontLoader.h"

class TextView : public QQuickRhiItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TextFramebuffer* framebuffer READ framebuffer WRITE setFramebuffer NOTIFY framebufferChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int posX READ posX WRITE setPosX NOTIFY posXChanged)
    Q_PROPERTY(int posY READ posY WRITE setPosY NOTIFY posYChanged)
    Q_PROPERTY(qreal fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(FontLoader* fontLoader READ fontLoader WRITE setFontLoader NOTIFY fontLoaderChanged)
public:
    explicit TextView(QQuickItem *parent = nullptr);

    TextFramebuffer *framebuffer() const;
    void setFramebuffer(TextFramebuffer *fb);

    FontLoader *fontLoader() const;
    void setFontLoader(FontLoader *loader);

    qreal fontSize() const;
    void setFontSize(qreal size);

    int width() const;
    void setWidth(int w);

    int height() const;
    void setHeight(int h);

    int posX() const;
    void setPosX(int x);

    int posY() const;
    void setPosY(int y);


signals:
    void framebufferChanged();
    void widthChanged();
    void heightChanged();
    void posXChanged();
    void posYChanged();
    void fontLoaderChanged();
    void fontSizeChanged();

protected:
    QQuickRhiItemRenderer *createRenderer() override;

private:
    TextFramebuffer *m_framebuffer = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_posX = 0;
    int m_posY = 0;
    FontLoader *m_fontLoader = nullptr;
    qreal m_fontSize = 12.0;

    void updateImplicitSize();
};
