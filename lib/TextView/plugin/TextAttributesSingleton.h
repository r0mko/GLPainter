#pragma once
#include <QObject>
#include <QtQml/qqml.h>
#include "QmlTextAttributes.h"

namespace ui::textview::qml {

class TextAttributesSingleton : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    Q_INVOKABLE TextAttributes create() const { return TextAttributes(); }
};

} // namespace ui::textview::qml
