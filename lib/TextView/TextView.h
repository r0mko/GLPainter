#pragma once

#include <QQuickRhiItem>
#include <QtQml/qqml.h>

class TextView : public QQuickRhiItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TextView(QQuickItem *parent = nullptr);

protected:
    QQuickRhiItemRenderer *createRenderer() override;
};
