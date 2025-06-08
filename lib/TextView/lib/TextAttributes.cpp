#include "TextAttributes.h"
#include "qglobal.h"
#include <QHash>

namespace ui::textview {
using TextAttribteStorage = QHash<size_t, const TextAttributes::Data *>;
Q_GLOBAL_STATIC(TextAttribteStorage, textAttributesStore)

namespace {

static size_t computeHash(const TextAttributes::Data &d)
{
    size_t h = qHash(d.background.rgba());
    h ^= qHash(d.color.rgba());
    h ^= qHash(d.outlineColor.rgba());
    h ^= qHash(static_cast<int>(d.outlineWidth * 1000));
    h ^= qHash(quintptr(d.font));
    return h;
}

} // namespace

TextAttributes::TextAttributes() = default;

TextAttributes::TextAttributes(const QColor &background,
                               const QColor &color,
                               const QColor &outlineColor,
                               float outlineWidth,
                               FontLoader *font)
{
    Data d{background, color, outlineColor, outlineWidth, font};
    m_data = get(d);
}

bool TextAttributes::isValid() const
{
    return m_data != nullptr;
}

const TextAttributes::Data &TextAttributes::data() const
{
    return *m_data;
}

const TextAttributes::Data *TextAttributes::get(const Data &d)
{
    auto h = computeHash(d);
    if (textAttributesStore->contains(h))
        return textAttributesStore->value(h);
    auto *ptr = new Data(d);
    textAttributesStore->insert(h, ptr);
    return ptr;
}

} // namespace ui::textview
