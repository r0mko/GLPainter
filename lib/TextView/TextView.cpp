#include "TextView.h"

TextView::TextView(QQuickItem *parent)
    : QQuickRhiItem(parent)
{
}

QQuickRhiItemRenderer *TextView::createRenderer()
{
    return nullptr;
}
