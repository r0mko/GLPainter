#pragma once
#include <QColor>
#include <QtQml/qqml.h>
#include "TextAttributes.h"
#include "FontLoaderObject.h"

namespace ui::textview::qml {

class TextAttributes : public ui::textview::TextAttributes
{
    Q_GADGET
    QML_VALUE_TYPE(textAttributes)
    QML_CONSTRUCTIBLE_VALUE
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QColor outlineColor READ outlineColor WRITE setOutlineColor)
    Q_PROPERTY(float outlineWidth READ outlineWidth WRITE setOutlineWidth)
    Q_PROPERTY(FontLoaderObject* fontLoader READ fontLoader WRITE setFontLoader)

public:
    TextAttributes();

    QColor background() const;
    void setBackground(const QColor &c);

    QColor color() const;
    void setColor(const QColor &c);

    QColor outlineColor() const;
    void setOutlineColor(const QColor &c);

    float outlineWidth() const;
    void setOutlineWidth(float w);

    FontLoaderObject* fontLoader() const;
    void setFontLoader(FontLoaderObject *fl);

private:
    FontLoaderObject *m_fontLoader = nullptr;
};

} // namespace ui::textview::qml
