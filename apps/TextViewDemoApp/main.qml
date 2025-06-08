import QtQuick
import UI.Editors

Window {
    id: root
    width: 800
    height: 600
    visible: true

    TextView {
        anchors.fill: parent
        renderWidth: 80
        renderHeight: 60
        framebuffer: _doc
    }

    TextFramebuffer {
        id: _doc
    }

    Component.onCompleted: {
        let attr = TextAttributes.create();
        attr.color = Qt.rgba(255, 255, 255, 255);
        attr.background = Qt.rgba(0, 0, 0, 0);
        attr.outlineColor = Qt.rgba(255, 0, 0, 255);
        attr.outlineWidth = 2.0;
        _doc.putText(0, 0, "HelloWorld", attr);
    }
}
