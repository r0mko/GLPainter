#include "TextAttributes.h"

#include <QHash>

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

static QHash<size_t, const TextAttributes::Data *> &store()
{
    static QHash<size_t, const TextAttributes::Data *> s;
    return s;
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
    m_data = insert(d);
}

bool TextAttributes::isValid() const
{
    return m_data != nullptr;
}

const TextAttributes::Data &TextAttributes::data() const
{
    return *m_data;
}

const TextAttributes::Data *TextAttributes::insert(const Data &d)
{
    auto h = computeHash(d);
    auto &s = store();
    if (s.contains(h))
        return s.value(h);
    auto *ptr = new Data(d);
    s.insert(h, ptr);
    return ptr;
}
