import QtQuick 2.0

import OwlComponents 1.0
import enums 1.0

/// Маленькая кнопка с рамкой.
/// По-молчанию - зеленая кнопка "Плюс".
Rectangle {
    readonly property int w: 20

    property int colorScheme: Enums.GreenColorScheme

    property alias image: buttonImg.source

    id: topRect

    signal clicked()

    width: w
    height: w
    function backgroundColor() {
        if (colorScheme === Enums.GreenColorScheme) {
            return CoreAPI.rgba(92, 184, 92, 0.5);
        }

        return CoreAPI.rgba(217, 65, 61, 0.5);
    }
    function borderColor() {
        if (colorScheme === Enums.GreenColorScheme) {
            return CoreAPI.rgba(92, 184, 92, 1);
        }

        return CoreAPI.rgba(217, 65, 61, 1);
    }

    border {
        color: borderColor()
        width: 2
    }
    radius: 5
    color: backgroundColor()
    Image {
        id: buttonImg
        anchors.fill: parent
        anchors.margins: 4
        sourceSize {
            width: parent.width
            height: parent.height
        }
        source: "qrc:/img/images/plus.svg"
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            cursorShape = Qt.PointingHandCursor;
        }
        onExited: {
            cursorShape = Qt.ArrowCursor;
        }
        onClicked: {
            topRect.clicked();
        }
    }
}
