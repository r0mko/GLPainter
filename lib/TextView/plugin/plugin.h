#pragma once

#include <QQmlExtensionPlugin>

namespace ui::textview::qml {

class UiEditorsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

} // namespace ui::textview::qml
