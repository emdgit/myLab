import QtQuick 2.0

import OwlComponents 1.0

import "Common.js" as Script

Item {
    id: topItem

    Text {
        id: dbName
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
        }

        horizontalAlignment: Text.AlignHCenter

        font.pixelSize: 22
        font.family: Script.menuTextFontFamily()

        text: "DataBase: " + CoreAPI.getDatabaseName()
    }

}
