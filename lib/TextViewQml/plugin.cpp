#include "plugin.h"
#include <qqml.h>
#include "TextView.h"
#include "DFGlyph.h"
#include "FontLoaderObject.h"
#include "TextFramebufferObject.h"

void UiEditorsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("UI.Editors"));
    qmlRegisterType<TextView>(uri, 1, 0, "TextView");
    qmlRegisterType<DFGlyph>(uri, 1, 0, "DFGlyph");
    qmlRegisterType<FontLoaderObject>(uri, 1, 0, "FontLoader");
    qmlRegisterType<TextFramebufferObject>(uri, 1, 0, "TextFramebuffer");
}
