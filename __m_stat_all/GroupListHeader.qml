import QtQuick 2.0

import "Common.js" as Script

/// Заголовок списка, отображающего дерево групп
Rectangle {
    id: topRect

    property alias isProfit: switchButton.isProfit

    /// Need to add new root group
    signal newGroupClicked()

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
    FramedButton {
        anchors {
            right: parent.right
            rightMargin: 15
            verticalCenter: parent.verticalCenter
        }
        onClicked: {
            topRect.newGroupClicked();
        }
    }
}
