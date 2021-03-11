import QtQuick 2.0

import "Common.js" as Script

/// Заголовок списка, отображающего дерево групп
Rectangle {
    height: 45
    color: Script.defaultUnhoveredColor()
    SpendProfitButton {
        id: switchButton
        anchors {
            left: parent.left
            leftMargin: 15
            verticalCenter: parent.verticalCenter
        }
    }
    GreenFrameButton {
        anchors {
            right: parent.right
            rightMargin: 15
            verticalCenter: parent.verticalCenter
        }
    }
}
