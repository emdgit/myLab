import QtQuick 2.0

import "Common.js" as Script

/// Кнопка с иконками
Rectangle {

    id: topRect

    property string hoverPic: ""
    property string defaultPic: ""
    property string unactivePic: ""

    property int m_width: 35
    property int m_height: 35

    width: m_width
    height: m_height

    radius: 6
    color: Script.hoveredColor()


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

    Image {
        id: img
        source: picture()
        anchors.fill: parent

        sourceSize.width: parent.width
        sourceSize.height: parent.height

        function picture() {
            if ( !topRect.enabled ) {
                return unactivePic;
            }

            if ( mArea.containsMouse ) {
                if ( hoverPic === "" ) {
                    return defaultPic;
                } else {
                    return hoverPic;
                }
            }

            return defaultPic;
        }
    }

    MouseArea {
        id: mArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: { topRect.clicked() }
    }
}
