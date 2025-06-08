#include "plugin.h"
#include <qqml.h>
#include "TextView.h"
#include "DFGlyph.h"
#include "FontLoaderObject.h"
#include "TextFramebufferObject.h"
#include "QmlTextAttributes.h"
#include "TextAttributesSingleton.h"

namespace ui::textview::qml {

void UiEditorsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("UI.Editors"));
    qmlRegisterType<TextView>(uri, 1, 0, "TextView");
    qmlRegisterType<DFGlyph>(uri, 1, 0, "DFGlyph");
    qmlRegisterType<FontLoaderObject>(uri, 1, 0, "FontLoader");
    qmlRegisterType<TextFramebufferObject>(uri, 1, 0, "TextFramebuffer");
    qmlRegisterUncreatableType<TextAttributes>(uri, 1, 0, "TextAttributes", QStringLiteral("Use TextAttributes.create()"));
    qmlRegisterSingletonType<TextAttributesSingleton>(uri, 1, 0, "TextAttributes", [](QQmlEngine *, QJSEngine *) -> QObject* { return new TextAttributesSingleton; });
}

} // namespace ui::textview::qml
