#pragma once

#include <QQmlEngine>
#include <QQuickPaintedItem>
#include <private/qdistancefield_p.h>

Q_MOC_INCLUDE("FontLoaderObject.h")

class FontLoaderObject;

class DFGlyph : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(FontLoaderObject *fontLoader READ fontLoader WRITE setFontLoader NOTIFY fontLoaderChanged FINAL)
    Q_PROPERTY(QChar character READ character WRITE setCharacter NOTIFY characterChanged FINAL)

public:
    explicit DFGlyph(QQuickItem *parent = nullptr);

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;
    FontLoaderObject *fontLoader() const;
    void setFontLoader(FontLoaderObject *fontLoader);
    QChar character() const;
    void setCharacter(const QChar &character);

signals:
    void fontLoaderChanged();
    void characterChanged();

private:
    void updateDF();
    void setDirty();
    FontLoaderObject *m_fontLoader = nullptr;
    QChar m_character;
    QPointF m_topLeft;
    QDistanceField m_df;
    bool m_dirty = false;
};
