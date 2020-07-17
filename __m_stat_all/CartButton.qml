import QtQuick 2.0

import "Common.js" as Script

// Кнопка с корзинкой и плюсом для добавления покупки
Rectangle {

    id: topRect

    // Размер одной стороны. Кнопка предполагается квадратной
    property int side: 100

    // Скрыть
    property bool hidden: true

    signal clicked()

    QtObject {
        id: _d
        property int imageSize: 0
    }

    width: side
    height: side

    Rectangle {
        readonly property int dx: 2
        id: imageCenter
        width: 2
        height: 2
        x: side/2 - width/2 + dx
        y: side/2 - width/2
    }

    Image {
        id: img

        anchors.centerIn: imageCenter

        sourceSize.width: _d.imageSize
        sourceSize.height: _d.imageSize

        source: "qrc:/img/images/cart.svg"

        Behavior on opacity {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InQuart
            }
        }

        Behavior on sourceSize.width {
            NumberAnimation {
                duration: 300
                easing.type: Easing.OutQuart
            }
        }

        Behavior on sourceSize.height {
            NumberAnimation {
                duration: 300
                easing.type: Easing.OutQuart
            }
        }
    }

    states: [
        State {
            name: "hidden"
            when: hidden === true
            PropertyChanges {
                target: img
                opacity: 0
            }
            PropertyChanges {
                target: _d
                imageSize: 0
            }
        },
        State {
            name: "shown"
            when: hidden === false
            PropertyChanges {
                target: img
                opacity: 1
            }
            PropertyChanges {
                target: _d
                imageSize: topRect.side * 0.7
            }
        }
    ]

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: {
            if (containsMouse) {
                img.source = "qrc:/img/images/cart_hovered.svg"
            } else {
                img.source = "qrc:/img/images/cart.svg"
            }
        }
        onPressed: {
            _d.imageSize = topRect.side * 0.63
        }
        onReleased: {
            _d.imageSize = topRect.side * 0.7
            topRect.clicked()
        }
    }
}
