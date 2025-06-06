#pragma once

#include <QQuickRhiItem>
#include <QtQml/qqml.h>
#include <QSizeF>
#include "TextFramebuffer.h"

class TextView : public QQuickRhiItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TextFramebuffer* framebuffer READ framebuffer WRITE setFramebuffer NOTIFY framebufferChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int posX READ posX WRITE setPosX NOTIFY posXChanged)
    Q_PROPERTY(int posY READ posY WRITE setPosY NOTIFY posYChanged)
    Q_PROPERTY(QSizeF characterSize READ characterSize WRITE setCharacterSize NOTIFY characterSizeChanged)
public:
    explicit TextView(QQuickItem *parent = nullptr);

    TextFramebuffer *framebuffer() const;
    void setFramebuffer(TextFramebuffer *fb);

    int width() const;
    void setWidth(int w);

    int height() const;
    void setHeight(int h);

    int posX() const;
    void setPosX(int x);

    int posY() const;
    void setPosY(int y);

    QSizeF characterSize() const;
    void setCharacterSize(const QSizeF &size);

signals:
    void framebufferChanged();
    void widthChanged();
    void heightChanged();
    void posXChanged();
    void posYChanged();
    void characterSizeChanged();

protected:
    QQuickRhiItemRenderer *createRenderer() override;

private:
    TextFramebuffer *m_framebuffer = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_posX = 0;
    int m_posY = 0;
    QSizeF m_characterSize;

    void updateImplicitSize();
};
