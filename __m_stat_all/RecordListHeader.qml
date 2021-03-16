import QtQuick 2.0

import "Common.js" as Script

/// Заголовок списка, отображающего список записей
Rectangle {
    property string groupName: ""

    id: topItem

    height: 45
    color: Script.defaultUnhoveredColor()

    Text {
        text: topItem.groupName
        font {
            family: Script.menuTextFontFamily()
            pixelSize: 19
        }
        color: Script.menuTextColor()
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 15
            bottom: parent.bottom
            right: plusButton.left
        }
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    FramedButton {
        id: plusButton
        anchors {
            right: parent.right
            rightMargin: 15
            verticalCenter: parent.verticalCenter
        }
    }
}

