#pragma once

#include <QQmlEngine>
#include <QQuickPaintedItem>
#include <private/qdistancefield_p.h>

Q_MOC_INCLUDE("FontLoader.h")

class FontLoader;

class DFGlyph : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(FontLoader *fontLoader READ fontLoader WRITE setFontLoader NOTIFY fontLoaderChanged FINAL)
    Q_PROPERTY(QChar character READ character WRITE setCharacter NOTIFY characterChanged FINAL)

public:
    explicit DFGlyph(QQuickItem *parent = nullptr);

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;
    FontLoader *fontLoader() const;
    void setFontLoader(FontLoader *fontLoader);
    QChar character() const;
    void setCharacter(const QChar &character);

signals:
    void fontLoaderChanged();
    void characterChanged();

private:
    void updateDF();
    void setDirty();
    FontLoader *m_fontLoader = nullptr;
    QChar m_character;
    QPointF m_topLeft;
    QDistanceField m_df;
    bool m_dirty = false;
};
