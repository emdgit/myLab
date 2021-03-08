import QtQuick 2.0

import OwlComponents 1.0

import "Common.js" as Script

/// Форма информации.
Item {
    id: topItem

    Rectangle {
        id: blackRect
        anchors.fill: parent
        color: "black"
    }

    Text {
        id: dbName
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
        }

        horizontalAlignment: Text.AlignHCenter

        font.pixelSize: 22
        font.family: Script.menuTextFontFamily()
        color: Script.menuTextColor()

        text: "DataBase: " + CoreAPI.getDatabaseName()
    }

}
