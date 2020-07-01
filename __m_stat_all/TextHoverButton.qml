import QtQuick 2.0

import "Common.js" as Script

/// Кнопка, при наведении на которую, текст становится жирным
Rectangle {

    id: topRect

    width: 80
    height: 35

    radius: 6
    color: Script.hoveredColor()

    property alias text: label.text

    signal clicked()

    Text {
        id: label
        anchors.fill: parent
        font.family: Script.menuTextFontFamily()
        font.bold: mArea.containsMouse
        color: Script.menuTextColor()
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: { topRect.clicked() }
    }
}
