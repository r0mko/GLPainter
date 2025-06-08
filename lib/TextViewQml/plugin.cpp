#include "plugin.h"
#include <qqml.h>
#include "TextView.h"
#include "DFGlyph.h"

void UiEditorsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("UI.Editors"));
    qmlRegisterType<TextView>(uri, 1, 0, "TextView");
    qmlRegisterType<DFGlyph>(uri, 1, 0, "DFGlyph");
}
