#include "QmlTextAttributes.h"

namespace ui::textview::qml {

TextAttributes::TextAttributes() = default;

QColor TextAttributes::background() const { return data().background; }
void TextAttributes::setBackground(const QColor &c) {
    *static_cast<ui::textview::TextAttributes*>(this) = ui::textview::TextAttributes(c, color(), outlineColor(), outlineWidth(), m_fontLoader ? m_fontLoader->loader() : nullptr);
}

QColor TextAttributes::color() const { return data().color; }
void TextAttributes::setColor(const QColor &c) {
    *static_cast<ui::textview::TextAttributes*>(this) = ui::textview::TextAttributes(background(), c, outlineColor(), outlineWidth(), m_fontLoader ? m_fontLoader->loader() : nullptr);
}

QColor TextAttributes::outlineColor() const { return data().outlineColor; }
void TextAttributes::setOutlineColor(const QColor &c) {
    *static_cast<ui::textview::TextAttributes*>(this) = ui::textview::TextAttributes(background(), color(), c, outlineWidth(), m_fontLoader ? m_fontLoader->loader() : nullptr);
}

float TextAttributes::outlineWidth() const { return data().outlineWidth; }
void TextAttributes::setOutlineWidth(float w) {
    *static_cast<ui::textview::TextAttributes*>(this) = ui::textview::TextAttributes(background(), color(), outlineColor(), w, m_fontLoader ? m_fontLoader->loader() : nullptr);
}

FontLoaderObject* TextAttributes::fontLoader() const { return m_fontLoader; }
void TextAttributes::setFontLoader(FontLoaderObject *fl) {
    m_fontLoader = fl;
    *static_cast<ui::textview::TextAttributes*>(this) = ui::textview::TextAttributes(background(), color(), outlineColor(), outlineWidth(), m_fontLoader ? m_fontLoader->loader() : nullptr);
}

} // namespace ui::textview::qml
